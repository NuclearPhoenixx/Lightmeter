/* This contains all the functions for the DS3231 real time clock */

#ifndef rtc_h
#define rtc_h

class RTC_DS3231
{
  public:
    RTC_DS3231(); //def constructor
  
    unsigned long get_unix_time();
    
    float getTemperature();
    void setA1Time(char A1Day, char A1Hour, char A1Minute, char A1Second, char AlarmBits, bool A1Dy, bool A1h12, bool A1PM);
    void setA2Time(char A2Day, char A2Hour, char A2Minute, char AlarmBits, bool A2Dy, bool A2h12, bool A2PM);
    void turnOnAlarm(char Alarm);
    void turnOffAlarm(char Alarm);
    bool checkAlarmEnabled(char Alarm);
    bool checkIfAlarm(char Alarm);
    
};

#endif
