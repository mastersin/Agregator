#ifndef _ACROBOT_ENCODER_H
#define _ACROBOT_ENCODER_H

#include "PinChange.h"

namespace ACRobot {

class EncoderInterface
{
  public:
    virtual long getCounter() = 0;
    virtual int getSpeed() = 0;
};

class PCIntEncoderInterface
{
  public:
    PCIntEncoderInterface(uint8_t pin, voidFuncPtr tick)
    {
      pinMode(pin, INPUT);
      PCattachInterrupt(pin, tick, CHANGE);
    }
};

class IntEncoderInterface
{
  public:
    IntEncoderInterface(uint8_t pin, voidFuncPtr tick)
    {
      pinMode(pin, INPUT);
      attachInterrupt(digitalPinToInterrupt(pin), tick, CHANGE);
    }
};

template<int pin>
class PCIntEncoder: public PCIntEncoderInterface, public EncoderInterface
{
  public:
    static void tick() {
      counter()++;
    }
    PCIntEncoder(): PCIntEncoderInterface(pin, tick) {}
    operator long() { return counter(); }

    long getCounter() {
      return counter();
    }
    int getSpeed()
    {
      static unsigned long _prev = 0;
      unsigned long current = millis();
      int value = counter() / (current - _prev);
      _prev = current;
      return value;
    }
  private:
    static long &counter() {
      static long _counter = 0;
      return _counter;
    }
};

template<int pin>
class IntEncoder: public IntEncoderInterface, public EncoderInterface
{
  public:
    static void tick() {
      counter()++;
    }
    IntEncoder(): IntEncoderInterface(pin, tick) {}
    operator long() { return counter(); }

    long getCounter() {
      return counter();
    }
    int getSpeed()
    {
      static unsigned long _prev = 0;
      unsigned long current = millis();
      int value = counter() / (current - _prev);
      _prev = current;
      return value;
    }
  private:
    static long &counter() {
      static long _counter = 0;
      return _counter;
    }
};

} // ACRobot namespace

#endif

