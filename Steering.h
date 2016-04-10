#ifndef _ACROBOT_STEERING_H
#define _ACROBOT_STEERING_H

#include "Motor.h"

namespace ACRobot {


class Steering: public DCMotorInterface
{
  public:
    Steering(DCMotorInterface &leftMotor, DCMotorInterface &rightMotor):
      _angle(0), _adjust(0), _leftMotor(leftMotor), _rightMotor(rightMotor) {}

    int16_t& operator= (int16_t power) { return set(power); }
    operator int16_t const () { return _power; }

    void setPower(int16_t power) {
      set(power);
    }
    void setAngle(int16_t angle) {
      if (angle > 180)
        angle = 180;
      if (angle < -180)
        angle = -180;
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
      _adjust = map(_angle, -90, 90, -_power, _power);
      _leftMotor = leftPower();
      _rightMotor = rightPower();
    }
    int16_t leftPower() {
      int16_t adjust = _adjust;
      if (_angle <= 0)
        return _power;
      if (_power < 0)
        adjust = -adjust;
      return _power + adjust;
    }
    int16_t rightPower() {
      int16_t adjust = _adjust;
      if (_angle >= 0)
        return _power;
      if (_power < 0)
        adjust = -adjust;
      return _power - adjust;
    }

    int16_t _angle;
    int16_t _adjust;

    DCMotorInterface &_leftMotor;
    DCMotorInterface &_rightMotor;
};

} // ACRobot namespace

#endif

