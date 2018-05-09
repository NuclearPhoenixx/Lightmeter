/* This contains all the functions for the TSL2591 light sensor */

#ifndef lightsensor_h
#define lightsensor_h

#include <Arduino.h>

class TSL2591
{
  public:
    TSL2591(byte gain, byte timing); //def constructor
    bool begin(); //setup
  
    void displaySensorDetails();
    bool configureSensor(char gain = -1, char timing = -1);
  
    uint16_t simpleRead(char spectrum = 0);
    uint16_t advancedRead(char spectrum = 0);
    float luxRead();
    
  private:
    char _gain; //gain and timing for the first config
    char _timing;
};

#endif
