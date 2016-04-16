#ifndef _ACROBOT_MOTOR_H
#define _ACROBOT_MOTOR_H

#include "Utils.h"

namespace ACRobot {

static const int16_t MAX_POWER = 255;
static const int16_t MIN_POWER = -MAX_POWER;

class MotorInterface: public PollingInterface
{
  public:
    MotorInterface(): _power(0) {}

    void setPower(int16_t power) {
      if (power > MAX_POWER)
        power = MAX_POWER;
      if (power < MIN_POWER)
        power = MIN_POWER;
      set(power);
    }

  protected:
    virtual int16_t& set(int16_t power) { return _power = power; };
    int16_t _power;
};

class DCMotor: public MotorInterface
{
  public:
    DCMotor(uint8_t directPin, uint8_t pwmPin): _new_power(0), _directPin(directPin), _pwmPin(pwmPin)
    {
      pinMode(directPin, OUTPUT);
      pinMode(pwmPin, OUTPUT);

      clearDigitalPin(directPin);
      clearDigitalPin(pwmPin);
    }

    const int16_t& operator= (int16_t power) {
      setPower(power);
      return _new_power;
    }
    operator int16_t const () { return _power; }
    bool poll();

  protected:
    int16_t& set(int16_t power) { return _new_power = power; };

  private:
    int16_t _new_power;
    uint8_t _directPin;
    uint8_t _pwmPin;
};

class DCMotorBreak: public DCMotor
{
  public:
    DCMotorBreak(uint8_t directPin, uint8_t pwmPin, uint8_t breakPin):
      DCMotor(directPin, pwmPin), _breakPin(breakPin)
    {
      pinMode(breakPin, OUTPUT);
      go();
    }

    const int16_t& operator= (int16_t power) { return set(power); };
    operator int16_t const () { return _power; }

    void stop() {
      setDigitalPin(_breakPin);
    }
    void go() {
      clearDigitalPin(_breakPin);
    }

  private:
    uint8_t _breakPin;
};

} // ACRobot namespace

#endif

