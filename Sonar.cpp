#include "Sonar.h"

namespace ACRobot {

int Sonar::readSensor()
{
  int duration, value;
  digitalWrite(_trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(_trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(_trigPin, LOW); 

  duration = pulseIn(_echoPin, HIGH, _maxDist * 29 / 5); 
  if (duration == 0 && _lastDuration > (_maxDist/2)) {
    value = _maxDist;
    _lastDuration = _maxDist * 29 / 5;
  } else {
    value = duration * 5 / 29;
    _lastDuration = duration;
  }

  return value;
}

} // ACRobot namespace

