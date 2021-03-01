double scaling[] = {leftCalibrate, leftCalibrate, rightCalibrate, rightCalibrate};
int gain = 16;
//0.785

int motorsControl[][4] = {
  {1, 0, 0, 1}, //Forward
  {0, 1, 1, 0}, //Backward
  {1, 0, 1, 0}, //Right
  {0, 1, 0, 1}, //Left
  {1, 1, 1, 1}, //Breaking
  {0, 0, 0, 0}  //Coasting
};

const int samples = 5;
uint32_t leftEncHist[samples + 1] = {0};
uint32_t rightEncHist[samples + 1] = {0};
unsigned long timeHist[samples + 1] = {0};
int absoluteSpeed = 255;
double speedAdjust[4] = {1, 1, 1, 1};
int turningDir = 0; //0 = none, 2 = right, 3 = left
unsigned long timeLastStraight = 0;
unsigned long turningMaxTime = 5000;


void setupMotion (void) {
  //setup PWM for motors
  ledcAttachPin(ciMotorLeftA, 1); // assign Motors pins to channels
  ledcAttachPin(ciMotorLeftB, 2);
  ledcAttachPin(ciMotorRightA, 3);
  ledcAttachPin(ciMotorRightB, 4);

  // Initialize channels
  // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
  // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
  ledcSetup(1, 20000, 8); // 20ms PWM, 8-bit resolution
  ledcSetup(2, 20000, 8);
  ledcSetup(3, 20000, 8);
  ledcSetup(4, 20000, 8);

}

void setupSamplingArrays() {
  for (int i = 0; i <= samples; i++) {
    leftEncHist[i] = 0;
    rightEncHist[i] = 0;
    timeHist[i] = 0;
  }
}

double getRightSpeed() {

  double speed = 0;

  for (int i = 1; i < samples; i++) {
    int d = rightEncHist[i] - rightEncHist[i - 1];
    int t = timeHist[i] - timeHist[i - 1];
    if (t == 0) continue;
    speed += (double)d / t;
  }

  return speed / samples;
}

double getLeftSpeed() {

  double speed = 0;

  for (int i = 1; i < samples; i++) {
    int d = leftEncHist[i] - leftEncHist[i - 1];
    int t = timeHist[i] - timeHist[i - 1];
    if (t == 0) continue;
    speed += (double)d / t;
  }

  return speed / samples;
}

void runMotors(int val, double speed) {

  speed = speed * 0.8;

  double currentRightSpeed = abs(getRightSpeed()); //speed in encoder ticks/ms, max about 0.25
  double currentLeftSpeed = abs(getLeftSpeed());

  double speedDelta = currentRightSpeed - currentLeftSpeed;

  speedAdjust[0] = 1 + (speedDelta / currentLeftSpeed) * gain;
  speedAdjust[1] = 1 + (speedDelta / currentLeftSpeed) * gain;

  speedAdjust[2] = 1 - (speedDelta / currentRightSpeed) * gain;
  speedAdjust[3] = 1 - (speedDelta / currentRightSpeed) * gain;


  for (int i = 0; i < 4; i++) {

    int motorPower = (int)(scaling[i] * (120 + speed * speedAdjust[i] * (255 - 120)));

    if (motorPower > 255) motorPower = 255;
    if (motorPower < 0) motorPower = 0;

    ledcWrite(i + 1, motorsControl[val][i] * motorPower);

    Serial.print(" ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(motorPower);
  }

  Serial.print("\tL: ");
  Serial.print(currentLeftSpeed);
  Serial.print("\tR: ");
  Serial.print(currentRightSpeed);
  Serial.print("\tDelta: ");
  Serial.println(speedDelta);

}
