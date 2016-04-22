#ifndef _ACROBOT_CONTROL_MOTOR_H
#define _ACROBOT_CONTROL_MOTOR_H

#include "Motor.h"
#include "Steering.h"
#include "Interval.h"
#include "Encoder.h"

namespace ACRobot {

template<class M, class E>
class ControlMotor: public MotorInterface
{
  static const long default_interval = 200;

  public:
    ControlMotor(M &m, E &e): _new_power(0), _interval(default_interval) {}

    const int16_t& operator= (int16_t power) {
      setPower(power);
      return _new_power;
    }
    operator int16_t const () { return _power; }
    bool poll();

  protected:
    int16_t& set(int16_t power) { return _new_power = power; };

  private:
    int _new_power;
    Interval _interval;
};


class EncoderSteering: public Steering
{
  static const long default_interval = 200;
  static const long minimal_speed = 50;

  public:
    EncoderSteering(MotorInterface &leftMotor, MotorInterface &rightMotor, EncoderInterface &leftEncoder, EncoderInterface &rightEncoder, unsigned long interval = default_interval):
      Steering(leftMotor, rightMotor), _leftEncoder(leftEncoder), _rightEncoder(rightEncoder), _interval(interval), _leftSpeed(), _rightSpeed(0) {}

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
    operator int16_t const () {
      return _power;
    }

    bool poll() {
      bool retval = false;
      if (_interval.poll()) {
        _leftSpeed = _leftEncoder.getSpeed();
        _rightSpeed = _rightEncoder.getSpeed();
      }
      if (_leftMotor.poll())
        retval = true;
      if (_rightMotor.poll())
        retval = true;
      return retval;
    }

    int16_t getLeftSpeed() {
      return _leftSpeed;
    }
    int16_t getRightSpeed() {
      return _rightSpeed;
    }

  protected:
    int16_t& set(int16_t power) {
      _power = power;
      updateAdjust();
      return _power;
    }

  private:
    void updateAdjust() {
      int16_t adjust = powerAdjust();
      _leftMotor.setPower(leftPower(adjust));
      _rightMotor.setPower(rightPower(adjust));
    }

    EncoderInterface &_leftEncoder;
    EncoderInterface &_rightEncoder;
    Interval _interval;
    int _leftSpeed;
    int _rightSpeed;
};

} // ACRobot namespace

#endif

