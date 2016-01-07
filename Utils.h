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

#include <inttypes.h>

namespace ACRobot {

void setDigitalPin(uint8_t pin);
void clearDigitalPin(uint8_t pin);
uint8_t getDigitalPin(uint8_t pin);

class PollingInterface
{
  public:
    void poll() {};
};

inline void waitForStart(uint8_t pin)
{
  pinMode(pin, INPUT_PULLUP);
  while(getDigitalPin(pin) == HIGH);
  pinMode(pin, INPUT);
}

} // ACRobot namespace

#endif
