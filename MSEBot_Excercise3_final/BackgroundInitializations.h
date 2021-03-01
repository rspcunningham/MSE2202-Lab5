
//Pin assignments


uint8_t byteValIR;
long times[10] = {0};
int runningState;
int testState;
int32_t targetAngle;
int32_t angleCorrection = -4;
const int scanningRotationTicks = 130;
double leftSpeed, rightSpeed;
int32_t leftEncTarget, rightEncTarget;
unsigned long lastLoopTime = 0, loopIncrement = 1, currentLoopTime = 0;

volatile uint32_t vui32test1;
volatile uint32_t vui32test2;

#include <esp_task_wdt.h>
#include <cstring>
#include <Adafruit_NeoPixel.h>
#include <Math.h>

void loopWEBServerButtonresponce(void);

const int CR1_ciMainTimer =  1000;
const int CR1_ciHeartbeatInterval = 500;
const int CR1_ciMotorRunTime = 1000;
const long CR1_clDebounceDelay = 50;
const long CR1_clReadTimeout = 220;

const uint8_t ci8RightTurn = 18;
const uint8_t ci8LeftTurn = 17;

unsigned char CR1_ucMainTimerCaseCore1;
uint8_t CR1_ui8LimitSwitch;

uint8_t CR1_ui8IRDatum;
uint8_t CR1_ui8WheelSpeed;
uint8_t CR1_ui8LeftWheelSpeed;
uint8_t CR1_ui8RightWheelSpeed;



unsigned char ucMotion_Direction;
unsigned char ucMotion_Speed;

const uint8_t cui8StartingSpeed = 140;

uint8_t ui8LeftWorkingSpeed = cui8StartingSpeed;
uint8_t ui8RightWorkingSpeed = cui8StartingSpeed;

unsigned char ucMotorState = 0;

double dManualSpeed;
double dForwardSpeed;
double dReverseSpeed;
double dLeftSpeed;
double dRightSpeed;

uint32_t CR1_u32Now;
uint32_t CR1_u32Last;
uint32_t CR1_u32Temp;
uint32_t CR1_u32Avg;

unsigned long CR1_ulLastDebounceTime;
unsigned long CR1_ulLastByteTime;

unsigned long CR1_ulMainTimerPrevious;
unsigned long CR1_ulMainTimerNow;

unsigned long CR1_ulMotorTimerPrevious;
unsigned long CR1_ulMotorTimerNow;
unsigned char ucMotorStateIndex = 0;

unsigned long CR1_ulHeartbeatTimerPrevious;
unsigned long CR1_ulHeartbeatTimerNow;

boolean btHeartbeat = true;
boolean btRun = false;
boolean btToggle = true;
int iButtonState;
int iLastButtonState = HIGH;

// Declare our SK6812 SMART LED object:
Adafruit_NeoPixel SmartLEDs(2, 25, NEO_GRB + NEO_KHZ400);
// Argument 1 = Number of LEDs (pixels) in use
// Argument 2 = ESP32 pin number
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

#include "Extras/0_Core_Zero.h"
#include "Extras/MyWEBserver.h"
#include "Extras/BreakPoint.h"
#include "Extras/WDT.h"
#include "Extras/NVS.h"
