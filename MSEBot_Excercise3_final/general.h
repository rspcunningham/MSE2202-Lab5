void setupWDT() {

  WDT_EnableFastWatchDogCore1();
  WDT_ResetCore1();
  WDT_vfFastWDTWarningCore1[0] = 0;
  WDT_vfFastWDTWarningCore1[1] = 0;
  WDT_vfFastWDTWarningCore1[2] = 0;
  WDT_vfFastWDTWarningCore1[3] = 0;
  WDT_ResetCore1();
  WDT_vfFastWDTWarningCore1[4] = 0;
  WDT_vfFastWDTWarningCore1[5] = 0;
  WDT_vfFastWDTWarningCore1[6] = 0;
  WDT_vfFastWDTWarningCore1[7] = 0;
  WDT_ResetCore1();
  WDT_vfFastWDTWarningCore1[8] = 0;
  WDT_vfFastWDTWarningCore1[9] = 0;
  WDT_ResetCore1();
}

void setServo(const int angle) {
  
  long dutyCycle = map(angle, 0, 180, 1675, 8050);
  ledcWrite(6, dutyCycle);
  
}

void pushButtonHandler() {

  int iButtonValue = digitalRead(ciPB1);  // read value of push button 1
  if (iButtonValue != iLastButtonState) { // if value has changed
    CR1_ulLastDebounceTime = millis();    // reset the debouncing timer
  }

  if ((millis() - CR1_ulLastDebounceTime) > CR1_clDebounceDelay) {
    if (iButtonValue != iButtonState) { // if the button state has changed
      iButtonState = iButtonValue;      // update current button state

      if (iButtonState == LOW) {
        ENC_ClearLeftOdometer();
        ENC_ClearRightOdometer();
        btRun = !btRun;
        Serial.println(btRun);

        if (!btRun) {
          stop();
        } else {
          start();
        }
      }
    }
  }
  iLastButtonState = iButtonValue; // store button state
}

void limitSwitchHandler() {
  if (!digitalRead(ciLimitSwitch)) {
    btRun = 0; // if limit switch is pressed stop bot
  }
}

void flashHeartbeatLED() {

  CR1_ulHeartbeatTimerNow = millis();

  if (CR1_ulHeartbeatTimerNow - CR1_ulHeartbeatTimerPrevious >= CR1_ciHeartbeatInterval) {
    CR1_ulHeartbeatTimerPrevious = CR1_ulHeartbeatTimerNow;
    btHeartbeat = !btHeartbeat;
    digitalWrite(ciHeartbeatLED, btHeartbeat);
  }
}




void updateEncoder() {
  for (int i = 0; i < samples; i++) {
    rightEncHist[i] = rightEncHist[i + 1];
    leftEncHist[i] = leftEncHist[i + 1];
    timeHist[i] = timeHist[i + 1];
  }

  rightEncHist[samples] = ENC_vi32RightOdometer;
  leftEncHist[samples] = ENC_vi32LeftOdometer;
  timeHist[samples] = currentLoopTime;

}




void setup() {
  Serial.begin(115200);
  Serial2.begin(2400, SERIAL_8N1, ciIRDetector);  // IRDetector on RX2 receiving 8-bit words at 2400 baud

  Core_ZEROInit();
  setupWDT();
  setupMotion();
  ENC_ClearLeftOdometer();
  ENC_ClearRightOdometer();

  setupSamplingArrays();

  pinMode(ciHeartbeatLED, OUTPUT);
  pinMode(ciPB1, INPUT_PULLUP);
  pinMode(ciLimitSwitch, INPUT_PULLUP);

  SmartLEDs.begin();                          // Initialize Smart LEDs object (required)
  SmartLEDs.clear();                          // Set all pixel colours to off
  SmartLEDs.show();                           // Send the updated pixel colours to the hardware

  ledcAttachPin(servoPin, 6);     
  ledcSetup(6, 50, 16);

  EEPROM.begin(scanningRotationTicks);

}
