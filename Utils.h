#ifndef _ACROBOT_UTILS_H
#define _ACROBOT_UTILS_H

#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #if defined(__AVR__)
    #include <avr/io.h>
  #endif
  #include "WProgram.h"
#endif

#define clear_bit(port,bit) \
    asm volatile (             \
        "cbi %0, %1" "\n\t" : : "I" (_SFR_IO_ADDR(port)), "I" (bit))

#define set_bit(port,bit) \
    asm volatile (             \
        "sbi %0, %1" "\n\t" : : "I" (_SFR_IO_ADDR(port)), "I" (bit))

#include <inttypes.h>

namespace ACRobot {

void setDigitalPin(uint8_t pin);
void clearDigitalPin(uint8_t pin);
uint8_t getDigitalPin(uint8_t pin);

void setOutputPin(uint8_t pin);
void setInputPin(uint8_t pin);

class PollingInterface
{
  public:
    virtual bool poll() { return false; };
};

class Waiting
{
  uint8_t pin;
public:
  Waiting(uint8_t waitingPin): pin(waitingPin)
  {
    pinMode(pin, INPUT_PULLUP);
  }
  ~Waiting()
  {
    clearDigitalPin(pin);
  }
  operator bool()
  {
    return getDigitalPin(pin) != HIGH;
  }
};

inline void waitForStart(uint8_t pin)
{
  pinMode(pin, INPUT_PULLUP);
  while(getDigitalPin(pin) == HIGH);
  clearDigitalPin(pin);
  setInputPin(pin);
}

inline void waitForStartWithLED(uint8_t pin)
{
  pinMode(pin, INPUT_PULLUP);
  while(getDigitalPin(pin) == HIGH);
  setOutputPin(pin);
  clearDigitalPin(pin);
}

} // ACRobot namespace

#endif
