/* This contains all the functions for the DS3231 real time clock */

#include "rtc.h" // include my header file

#include <RTClib.h> //DS3231 lib
#include <EEPROM.h> //for EEPROM storage

RTC_DS3231 RTC_DS3231;

/* DEF CONSTRUCTOR */
DS3231::DS3231()
{
  //nothing to do for the constructor :c
}

bool DS3231::begin()
{
  return RTC_DS3231.begin();
}

bool DS3231::lostPower()
{
  return RTC_DS3231.lostPower();
}

void DS3231::setTime()
{
  DateTime createDate = DateTime(F(__DATE__), F(__TIME__)); //get the creation date
  RTC_DS3231.adjust(createDate); //set the RTC to the creation date
  EEPROM.put(0, createDate.unixtime()); //upload the creation time unixtime to EEPROM
}

uint32_t DS3231::unixtime()
{
  DateTime now = RTC_DS3231.now();
  return now.unixtime();
}

