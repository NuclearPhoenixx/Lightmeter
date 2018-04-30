/* This contains all the functions for the DS3231 real time clock */

#include "rtc.h" // include my header file

#include <DS3231.h> //DS3231 lib

RTClib RTClib;
DS3231 RTC;

/* DEF CONSTRUCTOR */
RTC_DS3231::RTC_DS3231()
{
  //nothing to do for the constructor :c
}

unsigned long RTC_DS3231::get_unix_time()
{
	DateTime now = RTClib.now();
	return now.unixtime();
}

/* BELOW ARE FUNCTIONS THAT GET REDIRECTED TO THE LIBRARY, JUST FOR THE SAKE OF LAZINESS */
float RTC_DS3231::getTemperature()
{
  return RTC.getTemperature();
}

void RTC_DS3231::setA1Time(char A1Day, char A1Hour, char A1Minute, char A1Second, char AlarmBits, bool A1Dy, bool A1h12, bool A1PM)
{
  // Set the details for Alarm 1
  RTC.setA1Time(A1Day, A1Hour, A1Minute, A1Second, AlarmBits, A1Dy, A1h12, A1PM);
}

void RTC_DS3231::setA2Time(char A2Day, char A2Hour, char A2Minute, char AlarmBits, bool A2Dy, bool A2h12, bool A2PM)
{
  // Set the details for Alarm 2
  RTC.setA2Time(A2Day, A2Hour, A2Minute, AlarmBits, A2Dy, A2h12, A2PM);
}

void RTC_DS3231::turnOnAlarm(char Alarm)
{
  // Enables alarm 1 or 2 and the external interrupt pin. If Alarm != 1, it assumes Alarm == 2.
  RTC.turnOnAlarm(Alarm);
}

void RTC_DS3231::turnOffAlarm(char Alarm)
{
  // Disables alarm 1 or 2 (default is 2 if Alarm != 1); and leaves the interrupt pin alone.
  RTC.turnOffAlarm(Alarm);
}

bool RTC_DS3231::checkAlarmEnabled(char Alarm)
{
  // Returns T/F to indicate whether the requested alarm is enabled. Defaults to 2 if Alarm != 1.
  return RTC.checkAlarmEnabled(Alarm);
}

bool RTC_DS3231::checkIfAlarm(char Alarm)
{
  // Checks whether the indicated alarm (1 or 2, 2 default); has been activated.
  return RTC.checkIfAlarm(Alarm);
}

