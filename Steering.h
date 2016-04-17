#ifndef _ACROBOT_STEERING_H
#define _ACROBOT_STEERING_H

#include "Motor.h"

namespace ACRobot {

static const int16_t MAX_ANGLE = 180;
static const int16_t MIN_ANGLE = -MAX_ANGLE;

class Steering: public MotorInterface
{
  public:
    Steering(MotorInterface &leftMotor, MotorInterface &rightMotor):
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

    void setAngle(int16_t angle) {
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
      int16_t angle = _angle > 0 ? _angle : -_angle;
      int16_t adjust = (angle * _power) / (MAX_ANGLE / 2);
      _leftMotor.setPower(leftPower(adjust));
      _rightMotor.setPower(rightPower(adjust));
    }
    int16_t leftPower(int16_t adjust) {
      if (_angle <= 0 && _power >=0)
        return _power;
      if (_angle >= 0 && _power <=0)
        return _power;
      return _power - adjust;
    }
    int16_t rightPower(int16_t adjust) {
      if (_angle >= 0 && _power >=0)
        return _power;
      if (_angle <= 0 && _power <=0)
        return _power;
      return _power - adjust;
    }

    int16_t _angle;
    MotorInterface &_leftMotor;
    MotorInterface &_rightMotor;
};

} // ACRobot namespace

#endif

