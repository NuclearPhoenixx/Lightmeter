/* This contains all the functions for the DS3231 real time clock */

#include "rtc.h" // include my header file

#include <DS3231.h> //DS3231 lib

RTClib RTC;

rtc::rtc(int addr)
{
  _addr = addr;
}

unsigned int rtc::get_unix_time()
{

	DateTime now = RTC.now();
	return now.unixtime();

}
