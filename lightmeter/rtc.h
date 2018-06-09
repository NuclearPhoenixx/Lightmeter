/* This contains all the functions for the DS3231 real time clock */

#ifndef rtc_h
#define rtc_h

#include <Arduino.h>

class DS3231
{
  public:
    DS3231(); //def constructor
    bool begin();

    bool lostPower();
    void setTime();
    uint32_t unixtime();
};

#endif


