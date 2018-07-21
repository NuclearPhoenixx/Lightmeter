/* This contains all the functions for the TSL2591 light sensor */

/*
 * TODO:
 *  * Use my slightly modified version of the Adafruit TSL2591 Library
 *    and further customize it.
 *  * Fix infinit loop if lux value is out of scope.
 *  * Increase lux return value consistency.
 */

#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include <Arduino.h>

class TSL2591
{
  public:
    TSL2591(byte gain, byte timing); //def constructor
    bool begin(); //setup
  
    void displaySensorDetails();
    void setGain(byte gain = -1);
    void setTiming(byte timing = -1);
    float luxRead();
    byte getGain();
    byte getTiming();
    
  private:
    char _gain; //gain and timing for the first config
    char _timing;
};

#endif

