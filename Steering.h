#ifndef _ACROBOT_STEERING_H
#define _ACROBOT_STEERING_H

#include "Motor.h"

namespace ACRobot {

static const int8_t MAX_ANGLE = 180;
static const int8_t MIN_ANGLE = -MAX_ANGLE;

static const int16_t MAX_POWER = 255;
static const int16_t MIN_POWER = -MAX_POWER;

class Steering: public DCMotorInterface
{
  public:
    Steering(DCMotorInterface &leftMotor, DCMotorInterface &rightMotor):
      _angle(0), _leftMotor(leftMotor), _rightMotor(rightMotor) {}

    const int16_t& operator= (int16_t power) {
      setPower(power);
      return _power;
    }
    const int16_t& operator+= (int16_t power) {
      setPower(_power + power);
      return _power;
    }
    const int16_t& operator-= (int16_t power) {
      setPower(_power - power);
      return _power;
    }
    operator int16_t const () { return _power; }

    void setPower(int16_t power) {
      if (power > MAX_POWER)
        power = MAX_POWER;
      if (power < MIN_POWER)
        power = MIN_POWER;
      set(power);
    }
    void setAngle(int8_t angle) {
      if (angle > MAX_ANGLE)
        angle = MAX_ANGLE;
      if (angle < MIN_ANGLE)
        angle = MIN_ANGLE;
      _angle = angle;
      updateAdjust();
    }

    bool poll() {
      bool retval = false;
      if (_leftMotor.poll())
        retval = true;
      if (_rightMotor.poll())
        retval = true;
      return retval;
    }

  protected:
    int16_t& set(int16_t power) {
      _power = power;
      updateAdjust();
      return _power;
    }

  private:
    void updateAdjust() {
      int8_t angle = _angle > 0 ? _angle : -_angle;
      int16_t _adjust = _power - (angle * _power) / (MAX_ANGLE / 2);
      _leftMotor = leftPower(_adjust);
      _rightMotor = rightPower(_adjust);
    }
    int16_t leftPower(int16_t adjust) {
      if (_angle <= 0)
        return _power;
      if (_power < 0)
        adjust = -adjust;
      return _power + adjust;
    }
    int16_t rightPower(int16_t adjust) {
      if (_angle >= 0)
        return _power;
      if (_power < 0)
        adjust = -adjust;
      return _power - adjust;
    }

    int8_t _angle;
    DCMotorInterface &_leftMotor;
    DCMotorInterface &_rightMotor;
};

} // ACRobot namespace

#endif

