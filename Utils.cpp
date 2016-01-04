#include "Utils.h"

namespace ACRobot {

void setDigitalPin(uint8_t pin)
{
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *out = portOutputRegister(port);

  *out |= bit;
}

void clearDigitalPin(uint8_t pin)
{
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *out = portOutputRegister(port);

  *out &= ~bit;
}

uint8_t getDigitalPin(uint8_t pin)
{
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);

  if (*portInputRegister(port) & bit) return HIGH;
  return LOW;
}

} // ACRobot namespace

