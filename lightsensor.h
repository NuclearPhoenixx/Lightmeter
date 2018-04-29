/* This contains all the functions for the TSL2591 light sensor */

#ifndef lightsensor_h
#define lightsensor_h

class lightsensor
{

  public:
    lightsensor(int addr); //setup function
  
    void displaySensorDetails();
    void configureSensor();
  
    void simpleRead();
    void advancedRead();
    void unifiedSensorAPIRead();
    
  private:
    int _addr;

};

#endif
