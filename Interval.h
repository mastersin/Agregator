#ifndef _ACROBOT_INTERVAL_H
#define _ACROBOT_INTERVAL_H

#include "Utils.h"

namespace ACRobot {

class Interval
{
  public:
    Interval(long ms): _interval(ms), _last(0) {}
    bool poll() { return poll(millis()); }
    bool poll(unsigned long ms);

  private:

    const unsigned long _interval;
    unsigned long _last;
};

} // ACRobot namespace

#endif

