#include "Motor.h"

using namespace ACRobot;

const uint8_t directA = 12;
const uint8_t directB = 13;
const uint8_t pwmA = 3;
const uint8_t pwmB = 11;
const uint8_t button = 2;

DCMotor mA(directA, pwmA);
DCMotor mB(directB, pwmB);

void poll()
{
  mA.poll();
  mB.poll();
}

void wait_for_start()
{
  pinMode(button, INPUT_PULLUP);
  while(getDigitalPin(button) == HIGH);
  pinMode(button, INPUT);
}

void setup()
{
  wait_for_start();
}

void loop()
{
  poll();
}
