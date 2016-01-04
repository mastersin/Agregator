#include "Motor.h"

namespace ACRobot {

void DCMotor::poll()
{
  if (_power == _new_power)
    return;

  if (_new_power >= 0 && _power <= 0)
    clearDigitalPin(_directPin);
  else if (_new_power < 0 && _power >= 0) {
    setDigitalPin(_directPin);
    _new_power = -_new_power;
  }

  _power = _new_power;
  analogWrite(_pwmPin, _power);
}

} // ACRobot namespace

