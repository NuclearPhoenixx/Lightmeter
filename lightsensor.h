/* This contains all the functions for the TSL2591 light sensor */

#ifndef lightsensor_h
#define lightsensor_h

#include <Arduino.h>

class TSL2591
{
  public:
    TSL2591(unsigned int gain, unsigned int timing); //def constructor
    bool begin(); //setup
  
    void displaySensorDetails();
    bool configureSensor(unsigned int gain = -1, unsigned int timing = -1);
  
    uint16_t simpleRead(unsigned int spectrum = 0);
    uint16_t advancedRead(unsigned int spectrum = 0);
    float luxRead();
    
  private:
    unsigned int _gain; //gain and timing for the first config
    unsigned int _timing;
};

#endif
