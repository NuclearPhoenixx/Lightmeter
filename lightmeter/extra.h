/* This contains all the extra utility functions for the lightmeter */

#ifndef extra_h
#define extra_h

#include <Arduino.h>

namespace extra
{
  void signal_led(byte flashes);
  void sleep(uint32_t sTime);
}

#endif

