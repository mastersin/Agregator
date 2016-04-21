#include "Steering.h"
#include "Sonar.h"
#include "Config.h"
#include "Interval.h"
#include "Encoder.h"

//#define DEBUG

using namespace ACRobot;

int16_t power = -145;

const uint8_t directA = 12;
const uint8_t directB = 13;
const uint8_t pwmA = 3;
const uint8_t pwmB = 11;
const uint8_t breakA = 9;
const uint8_t breakB = 8;
const uint8_t button = 2;

struct Settings
{
  uint8_t status;
  uint32_t time;
};

Settings settings = { 0, 0 };
Config<Settings> config(0x01, settings);

DCMotorBreak mA(directA, pwmA, breakA);
DCMotorBreak mB(directB, pwmB, breakB);
Steering mControl(mA, mB);

const uint8_t trigSonarPin = 4;
const uint8_t leftSonarPin = 7;
const uint8_t rightSonarPin = 10;

Sonar leftSonar(trigSonarPin, leftSonarPin);
Sonar rightSonar(trigSonarPin, rightSonarPin);

PCIntEncoder<A2> encoderA;
PCIntEncoder<A3> encoderB;

enum IntervalType {
    GLOBAL,
    SONARS,
    ENCODERS,
    BLINK,
    CONFIG,
    NUMBER_OF_INTERVALS
};

const unsigned long GLOBAL_INTERVAL   = 10;
const unsigned long SONARS_INTERVAL   = 100;
const unsigned long ENCODERS_INTERVAL = 200;
const unsigned long BLINK_INTERVAL    = 1000;
const unsigned long CONFIG_INTERVAL   = 10000;

Intervals<NUMBER_OF_INTERVALS> intervals;

int poll()
{
  mControl.poll();

  return intervals.status();
}

void setup()
{
#ifdef DEBUG
  Serial.begin(9600);
#endif

  settings = config();

  intervals[GLOBAL]   = GLOBAL_INTERVAL;
  intervals[SONARS]   = SONARS_INTERVAL;
  intervals[ENCODERS] = ENCODERS_INTERVAL;
  intervals[BLINK]    = BLINK_INTERVAL;
  intervals[CONFIG]   = CONFIG_INTERVAL;

#ifdef DEBUG
  Serial.println("wait");
#endif

  waitForStartWithLED(button);

#ifdef DEBUG
  Serial.println("start");
#endif
}

void logic()
{
  if (leftSonar() < 100 || rightSonar() < 100) {
    mControl = -power;
  } else {
    mControl = power;
  }
}

void sonars()
{
  static bool flipflop = false;

  if (flipflop)
    leftSonar.poll();
  else
    rightSonar.poll();

  flipflop = !flipflop;
}

void encoders()
{
  int speedA = encoderA.getSpeed();
  int speedB = encoderB.getSpeed();

#ifdef DEBUG
  Serial.print("motorA = ");
  Serial.print(mA);
  Serial.print(", encoderA = ");
  Serial.print(encoderA);
  Serial.print(", sppedA = ");
  Serial.println(speedA);

  Serial.print("motorB = ");
  Serial.print(mB);
  Serial.print(", encoderB = ");
  Serial.print(encoderB);
  Serial.print(", sppedB = ");
  Serial.println(speedB);
#endif
}

void blink()
{
  static bool led = true;
  static int angle = 0;

  if (led)
    clearDigitalPin(button);
  else
    setDigitalPin(button);

  mControl.setAngle(angle);

  led = !led;
  angle = -angle;

#ifdef DEBUG
  Serial.print("left sonar = ");
  Serial.println(leftSonar());
  Serial.print("right sonar = ");
  Serial.println(rightSonar());
  Serial.println();
#endif
}

void loop()
{
  switch (poll())
  {
    case GLOBAL:
      logic();
      break;
    case SONARS:
      sonars();
      break;
    case ENCODERS:
      encoders();
      break;
    case BLINK:
      blink();
      break;
    case CONFIG:
      config.poll();
      break;
  }
}
