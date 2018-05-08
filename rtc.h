/* This contains all the functions for the DS3231 real time clock */

#ifndef rtc_h
#define rtc_h

class DS3231
{
  public:
    DS3231(); //def constructor
    bool begin();

    bool lostPower();
    void setTime();
    unsigned long unixtime();
};

#endif
