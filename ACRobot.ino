#include "Motor.h"
#include "Sonar.h"
#include "Interval.h"

using namespace ACRobot;

uint8_t power = 75;

const uint8_t directA = 12;
const uint8_t directB = 13;
const uint8_t pwmA = 3;
const uint8_t pwmB = 11;
const uint8_t button = 2;

DCMotor mA(directA, pwmA);
DCMotor mB(directB, pwmB);

const uint8_t trigSonar = 8;
const uint8_t echoSonar = 9;

Sonar sonar(trigSonar, echoSonar);

enum State {
    GLOBAL = 0,
    SONARS = 1,
    BLINK  = 2
};

const unsigned long GLOBAL_INTERVAL = 10;
const unsigned long SONARS_INTERVAL = 100;
const unsigned long BLINK_INTERVAL  = 1000;

const uint8_t NUMBER_OF_INTERVALS = 3;
Intervals<NUMBER_OF_INTERVALS> intervals;

int poll()
{
  mA.poll();
  mB.poll();

  return intervals.poll();
}

void setup()
{
  intervals[GLOBAL] = GLOBAL_INTERVAL;
  intervals[SONARS] = SONARS_INTERVAL;
  intervals[BLINK]  = BLINK_INTERVAL;
  waitForStart(button);
}

void logic()
{
  if (sonar() < 50) {
    mA = 0;
    mB = 0;
  } else {
    mA = power;
    mB = power;
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
  }
}
