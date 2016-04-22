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

class RotateEncoderInterface
{
  public:
    RotateEncoderInterface(uint8_t pin, uint8_t dir, voidFuncPtr tick)
    {
      pinMode(pin, INPUT);
      pinMode(dir, INPUT);
      PCattachInterrupt(pin, tick, FALLING);
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
      int value = (counter() - _prev);
      _prev = counter();
      return value;
    }
    void reset() { counter() = 0; }
  protected:
    static volatile long &counter() {
      static volatile long _counter = 0;
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
      int value = (counter() - _prev);
      _prev = counter();
      return value;
    }
    void reset() { counter() = 0; }
  protected:
    static volatile long &counter() {
      static volatile long _counter = 0;
      return _counter;
    }
};

template<int pin, int dir>
class RotateEncoder: public RotateEncoderInterface
{
  public:
    static void tick() {
      if (digitalRead(dir))
        counter()++;
      else
        counter()--;
    }
    RotateEncoder(): RotateEncoderInterface(pin, dir, tick) {}
    operator long() { return counter(); }

    long getCounter() {
      return counter();
    }
    void reset() { counter() = 0; }
  private:
    static volatile long &counter() {
      static volatile long _counter = 0;
      return _counter;
    }
};

} // ACRobot namespace

#endif

