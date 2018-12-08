/* This contains all the support functionality for the lightmeter */

#ifndef SUPPORT_H
#define SUPPORT_H

#include <Arduino.h>

namespace support
{
  void ledFlash();
  void sleep(uint32_t sleepTime);
}

#endif
