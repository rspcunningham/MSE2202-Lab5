
double leftCalibrate = 1;
double rightCalibrate = 0.85;


#include "Pinout.h";
#include "BackgroundInitializations.h";
#include "IRHandler.h";
#include "motors.h";


void start() {
  runningState = 0;
  times[0] = millis();
}

void stop() {
  runMotors(5, 1);
}

#include "general.h";

double speed = 0;

void loop() {

  //Routine Tasks
  ENC_Averaging(); //No idea what it does, just keep it in and ignore lol.
  pushButtonHandler(); //Toggles between btRun = true and btRun = false when button is clicked
  limitSwitchHandler(); //sets btRun = false if the front limit switch is hit
  flashHeartbeatLED(); //Flashes the blue LED on the ESP32

  byteValIR = getIRData(); //Pulls data from the IR receiver

  currentLoopTime = millis();
  if (currentLoopTime < lastLoopTime + loopIncrement) return;
  lastLoopTime = currentLoopTime;

  updateEncoder(); //adds encoder and time information to arrays used to calculate speed

  if (btRun) {

    int runLength = 0;

    switch (runningState) {

      case 0: //2000 ms hold on start
        //Serial.println(0);
        if (millis() >= times[0] + 2000) runningState = 1;
        times[1] = millis();

        SmartLEDs.setPixelColor(0, 255, 255, 255);
        SmartLEDs.setPixelColor(1, 255, 255, 255);
        SmartLEDs.show();

        setServo(180);

        break;


      case 1: //drive forwards
        runMotors(0, 1);
        runLength = 240;
        if (abs(ENC_vi32LeftOdometer) >= runLength || abs(ENC_vi32RightOdometer) >= runLength) {
          ENC_ClearLeftOdometer();
          ENC_ClearRightOdometer();
          runningState = 2;
        }

        times[2] = millis();

        SmartLEDs.setPixelColor(0, 0, 255, 0);
        SmartLEDs.setPixelColor(1, 0, 255, 0);
        SmartLEDs.show();

        break;

      case 2: //turn left
        runMotors(3, 0.5);
        if (abs(ENC_vi32LeftOdometer) > 15) {
          ENC_ClearLeftOdometer();
          ENC_ClearRightOdometer();
          runningState = 3;
        }
        times[3] = millis();
        break;

      case 3: //driving forwards
        runMotors(0, 1);
        runLength = 250;
        if (abs(ENC_vi32LeftOdometer) >= runLength || abs(ENC_vi32RightOdometer) >= runLength) {
          ENC_ClearLeftOdometer();
          ENC_ClearRightOdometer();
          runningState = 4;
        }
        times[4] = millis();

        SmartLEDs.setPixelColor(0, 255, 0, 0);
        SmartLEDs.setPixelColor(1, 255, 0, 0);
        SmartLEDs.show();

        break;

      case 4: // turn right
        runMotors(2, 0.5);
        if (ENC_vi32LeftOdometer < -38) {
          ENC_ClearLeftOdometer();
          ENC_ClearRightOdometer();
          runningState = 5;
        }
        times[5] = millis();

        SmartLEDs.setPixelColor(0, 255, 255, 255);
        SmartLEDs.setPixelColor(1, 255, 255, 255);
        SmartLEDs.show();

        break;

      case 5: // fowards 
        runMotors(0, 1);
        runLength = 280;
        if (abs(ENC_vi32LeftOdometer) >= runLength || abs(ENC_vi32RightOdometer) >= runLength) {
          ENC_ClearLeftOdometer();
          ENC_ClearRightOdometer();
          runningState = 6;
        }
        times[6] = millis();
        break;

      case 6: // right turn
        runMotors(2, 0.5);
        if (ENC_vi32LeftOdometer < -38) {
          ENC_ClearLeftOdometer();
          ENC_ClearRightOdometer();
          runningState = 7;
        }
        times[7] = millis();

        SmartLEDs.setPixelColor(0, 255, 255, 255);
        SmartLEDs.setPixelColor(1, 255, 255, 255);
        SmartLEDs.show();

        break;

      case 7: //forwards to align with beacon
        runMotors(0, 1);
        runLength = 300;
        if (abs(ENC_vi32LeftOdometer) >= runLength || abs(ENC_vi32RightOdometer) >= runLength) {
          ENC_ClearLeftOdometer();
          ENC_ClearRightOdometer();
          runningState = 8;
        }
        times[8] = millis();
        break;

      case 8: //left turn until beacon is seen
        runMotors(3, 0.5);
        if (byteValIR == 0x41) {
          ENC_ClearLeftOdometer();
          ENC_ClearRightOdometer();
          runningState = 9;
        }
        times[9] = millis();
        break;

      case 9: // go a little to get the centre of the IR input
        runMotors(3, 0.5);
        if (abs(ENC_vi32LeftOdometer) > 3) {
          ENC_ClearLeftOdometer();
          ENC_ClearRightOdometer();
          runningState = 10;
        }
        break;

      case 10:
        runMotors(0, 1);
        if (getIRData() == 0x41) {
          runningState = 11;
          leftEncTarget = abs(ENC_vi32LeftOdometer);
          rightEncTarget = abs(ENC_vi32RightOdometer);
          ENC_ClearLeftOdometer();
          ENC_ClearRightOdometer();
        }
        break;

      case 11: 
        runMotors(1, 1);
        if (abs(ENC_vi32LeftOdometer) >= leftEncTarget || abs(ENC_vi32RightOdometer) >= rightEncTarget) {
          ENC_ClearLeftOdometer();
          ENC_ClearRightOdometer();
          runningState = 12;
        }
        break;

      case 12: //turn 180º
        runMotors(2, 0.2); //speed should be 0.05
        if (ENC_vi32LeftOdometer < -40) runningState = 13;
        break;

      case 13: //finish
        runMotors(5, 0);
        setServo(90);
        btRun = !btRun;
        stop();
        break;

    }
  } else {
    SmartLEDs.setPixelColor(0, 0, 0, 0);
    SmartLEDs.setPixelColor(1, 0, 0, 0);
    SmartLEDs.show();
  }

}
