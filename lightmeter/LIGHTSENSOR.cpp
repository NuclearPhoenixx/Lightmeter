/* This contains all the functions for the TSL2591 light sensor */

#include "LIGHTSENSOR.h" // include my header file
#include <Adafruit_TSL2591.h> //TSL2591 lib

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)

/* DEF CONSTRUCTOR */
TSL2591::TSL2591(byte gain, byte timing)
{
  _gain = gain;
  _timing = timing;
}

/* SETUP FUNCTION */
bool TSL2591::begin()
{
  if(tsl.begin())
  {
    TSL2591::setGain(_gain); //first config with the vars from the constructor
    TSL2591::setTiming(_timing);
    return true;
  }
  return false; //if no light sensor there, return false
}

/* Display basic info on this sensor from the unified sensor API sensor_t type (Adafruit_Sensor) */
void TSL2591::displaySensorDetails()
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" lux"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value, 6); Serial.println(F(" lux"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution, 6); Serial.println(F(" lux"));  
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
}

/* Array with all the gain variables */
byte lightsensorGains[4] = {
                           TSL2591_GAIN_LOW, // 1x gain
                           TSL2591_GAIN_MED, // 25x gain
                           TSL2591_GAIN_HIGH, // 428x gain
                           TSL2591_GAIN_MAX // 9876x gain
                          };

/* Set the lightsensor gain */
void TSL2591::setGain(byte gain)
{
  if(gain < 0 || gain > 4)
  {
    return;
  }
  _gain = gain; //Update private gain var

  tsl.setGain(lightsensorGains[gain]);
}

/* Array with all the integration timing variables */
byte lightsensorTimings[6] = {
                              TSL2591_INTEGRATIONTIME_100MS,
                              TSL2591_INTEGRATIONTIME_200MS,
                              TSL2591_INTEGRATIONTIME_300MS,
                              TSL2591_INTEGRATIONTIME_400MS,
                              TSL2591_INTEGRATIONTIME_500MS,
                              TSL2591_INTEGRATIONTIME_600MS
                             };

/* Set the lightsensor timing */
void TSL2591::setTiming(byte timing)
{
  if(timing < 0 || timing > 6)
  {
    return;
  }
  _timing = timing; //Update private timing var

  tsl.setTiming(lightsensorTimings[timing]);
}

/* Calculates the current visible Lux value and then sends the sensor back to sleep */
float TSL2591::luxRead()
{
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t full = lum & 0xFFFF;
  
  while(full >= 65534) //decrease timing or gain if close to overflowing (65535)
  {
    if(_timing == 0) //if timing already at minimum decrease gain
    {
      if(_gain == 0) //if gain also already at minimum break
      {
        break;
      }
      else //else decrease gain
      {
        TSL2591::setGain(_gain - 1);
      }
    }
    else //else decrease timing
    {
      TSL2591::setTiming(_timing - 1);
    }

    delay(600); //calm down lightsensor, worst case length
    lum = tsl.getFullLuminosity(); //get new values after the changes
    full = lum & 0xFFFF;
  }

  while(full <= 1) //increase gain or timing if close to detecting nothing
  {
    if(_gain == 3) //if gain is already max increase the timing
    {
      if(_timing == 5) //if timing also already at maximum break
      {
        break;
      }
      else //else increase timing
      {
        TSL2591::setTiming(_timing + 1);
      }
    }
    else //else increase gain
    {
      TSL2591::setGain(_gain + 1);
    }

    delay(600); //calm down lightsensor, worst case length
    lum = tsl.getFullLuminosity(); //get new values after the changes
    full = lum & 0xFFFF;
  }

  uint16_t ir = lum >> 16;
  
  return tsl.calculateLux(full, ir);
}

/* Returns the current lightsensor gain */
byte TSL2591::getGain()
{
  return TSL2591::_gain;
}

/* Returns the current lightsensor timing */
byte TSL2591::getTiming()
{
  return TSL2591::_timing;
}

