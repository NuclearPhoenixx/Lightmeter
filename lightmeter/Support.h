/* This contains all the support functionality for the lightmeter */

#ifndef SUPPORT_H
#define SUPPORT_H

#include <Arduino.h>

#define LED_BUILTIN 2 //Pin for the status LED.

namespace support
{
void ledFlash();
void sleep(uint32_t sleepTime);
}

#endif
