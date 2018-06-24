/* This contains all the extra utility functions for the lightmeter */

#ifndef EXTRA_H
#define EXTRA_H

#include <Arduino.h>

namespace extra
{
  void signal_led(byte flashes);
  void sleep(uint32_t sTime);
}

#endif

