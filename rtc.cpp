/* This contains all the functions for the DS3231 real time clock */

#include "rtc.h" // include my header file

#include <RTClib.h> //DS3231 lib

RTC_DS3231 RTC_DS3231;

/* DEF CONSTRUCTOR */
DS3231::DS3231()
{
  //nothing to do for the constructor :c
}

bool DS3231::begin()
{
  return RTC_DS3231.begin(); //begin the RTClib
}

bool DS3231::lostPower()
{
  return RTC_DS3231.lostPower(); //check DS3231 status register for lost power
}

uint32_t DS3231::unixtime()
{
  DateTime theTime = RTC_DS3231.now();
  return theTime.unixtime();
}

