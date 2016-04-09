#ifndef _ACROBOT_MOTOR_H
#define _ACROBOT_MOTOR_H

#include "Utils.h"

namespace ACRobot {

class DCMotorInterface: public PollingInterface
{
  public:
    DCMotorInterface(): _power(0) {}
    const int16_t& operator= (int16_t power) { return set(power); };

  protected:
    virtual int16_t& set(int16_t power) { return _power = power; };
    int16_t _power;
};

class DCMotor: public DCMotorInterface
{
  public:
    DCMotor(uint8_t directPin, uint8_t pwmPin): _new_power(0), _directPin(directPin), _pwmPin(pwmPin)
    {
      pinMode(directPin, OUTPUT);
      pinMode(pwmPin, OUTPUT);

      clearDigitalPin(directPin);
      clearDigitalPin(pwmPin);
    }

    const int16_t& operator= (int16_t power) { return set(power); };
    operator int16_t const () { return _power; }
    bool poll();

  protected:
    int16_t& set(int16_t power) { return _new_power = power; };

  private:
    int16_t _new_power;
    uint8_t _directPin;
    uint8_t _pwmPin;
};

} // ACRobot namespace

#endif

