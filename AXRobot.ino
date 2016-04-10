#include "Steering.h"
#include "Sonar.h"
#include "Config.h"
#include "Interval.h"

using namespace ACRobot;

int16_t power = 75;

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

const uint8_t trigSonar = 4;
const uint8_t echoSonar = 7;

Sonar sonar(trigSonar, echoSonar);

enum State {
    GLOBAL = 0,
    SONARS = 1,
    BLINK  = 2,
    CONFIG = 3
};

const unsigned long GLOBAL_INTERVAL = 10;
const unsigned long SONARS_INTERVAL = 100;
const unsigned long BLINK_INTERVAL  = 1000;
const unsigned long CONFIG_INTERVAL = 10000;

const uint8_t NUMBER_OF_INTERVALS = 4;
Intervals<NUMBER_OF_INTERVALS> intervals;

int poll()
{
  mControl.poll();

  return intervals.status();
}

void setup()
{
  settings = config();

  intervals[GLOBAL] = GLOBAL_INTERVAL;
  intervals[SONARS] = SONARS_INTERVAL;
  intervals[BLINK]  = BLINK_INTERVAL;
  intervals[CONFIG] = CONFIG_INTERVAL;

  waitForStartWithLED(button);
}

void logic()
{
  if (sonar() < 50) {
    mControl = -power;
  } else {
    mControl = power;
  }
}

void blink()
{
    power = -power;
}

void loop()
{
  switch (poll())
  {
    case GLOBAL:
      logic();
      break;
    case SONARS:
      sonar.poll();
      break;
    case BLINK:
      blink();
      break;
    case CONFIG:
      config.poll();
      break;
  }
}
