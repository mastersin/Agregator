#include "Utils.h"

namespace ACRobot {

void setDigitalPin(uint8_t pin)
{
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *out = portOutputRegister(port);

  register uint8_t oldSREG = SREG;
  cli();

  *out |= bit;

  SREG = oldSREG;
}

void clearDigitalPin(uint8_t pin)
{
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *out = portOutputRegister(port);

  register uint8_t oldSREG = SREG;
  cli();

  *out &= ~bit;

  SREG = oldSREG;
}

uint8_t getDigitalPin(uint8_t pin)
{
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);

  if (*portInputRegister(port) & bit) return HIGH;
  return LOW;
}

void setOutputPin(uint8_t pin)
{
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *reg = portModeRegister(port);

  uint8_t oldSREG = SREG;
  cli();

  *reg |= bit;

  SREG = oldSREG;
}

void setInputPin(uint8_t pin)
{
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *reg = portModeRegister(port);

  uint8_t oldSREG = SREG;
  cli();

  *reg &= ~bit;

  SREG = oldSREG;
}

} // ACRobot namespace

