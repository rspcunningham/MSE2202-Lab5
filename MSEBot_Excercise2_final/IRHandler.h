int IRMap[scanningRotationTicks] = {0};

uint8_t getIRData() {
  if (Serial2.available() > 0) {               // check for incoming data
    CR1_ui8IRDatum = Serial2.read();          // read the incoming byte
    //Serial.println(CR1_ui8IRDatum, HEX);        // uncomment to output received character
    CR1_ulLastByteTime = millis();            // capture time last byte was received
  } else {
    // check to see if elapsed time exceeds allowable timeout
    if (millis() - CR1_ulLastByteTime > CR1_clReadTimeout) {
      CR1_ui8IRDatum = 0;                     // if so, clear incoming byte
    }
  }

  return CR1_ui8IRDatum;
}


void handleIR(uint8_t val) {
  if (val == 0x55) {                // if proper character is seen
    SmartLEDs.setPixelColor(0, 0, 25, 0);      // make LED1 green with 10% intensity
  }
  else if (val == 0x41) {           // if "hit" character is seen
    SmartLEDs.setPixelColor(0, 25, 0, 25);     // make LED1 purple with 10% intensity
  }
  else {                                       // otherwise
    SmartLEDs.setPixelColor(0, 25, 0, 0);      // make LED1 red with 10% intensity
  }

  SmartLEDs.show();                            // send updated colour to LEDs
}

void writeIRMap(int encoderTick, uint8_t charRecieved) {
  if (encoderTick >= 130 || encoderTick < 0) {
    btRun = !btRun;
    return;
  }
  
  if (IRMap[encoderTick] == 0) {

    if (charRecieved == 0x55) {
      IRMap[encoderTick] = 1;
    } else {
      IRMap[encoderTick] = 0;
    }

    //IRMap[encoderTick] = getIRData();
    //IRMap[encoderTick] = encoderTick;

    /*
      Serial.print("Tick: ");
      Serial.print(encoderTick);
      Serial.print("\t IR: ");
      Serial.println(IRMap[encoderTick]);
    */
  }
}

int32_t getIRCentre(int mapping[]) {

  int sum = 0;
  int numElements = 0;

  for (int i = 0; i < scanningRotationTicks; i++) {
    sum += mapping[i] * i;
    numElements += mapping[i];
  }

  if (sum == 0) return scanningRotationTicks;
  else return sum / numElements;
}
