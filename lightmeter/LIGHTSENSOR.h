/* This contains all the functions for the TSL2591 light sensor */

#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include <Arduino.h>

class TSL2591
{
  public:
    TSL2591(byte gain, byte timing, byte tries); //def constructor
    bool begin(); //setup
  
    void displaySensorDetails();
    void setGain(byte gain = -1);
    void setTiming(byte timing = -1);
    float luxRead();
    byte getGain();
    byte getTiming();
    
  private:
    byte _gain; //gain and timing for the first config
    byte _timing;
    byte _max_tries;
};

#endif
