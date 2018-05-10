/* This contains all the functions for the TSL2591 light sensor */

#include "lightsensor.h" // include my header file
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
  if(tsl.begin()) {
    return TSL2591::configureSensor(_gain, _timing); //first config with the vars from the constructor
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
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution, 4); Serial.println(F(" lux"));  
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
}

/* Configure gain and integration time for the TSL2591 */
bool TSL2591::configureSensor(char gain, char timing)
{
  if(gain == -1) {gain = _gain;}
  if(timing == -1) {timing = _timing;}

  _gain = gain; //Update private vars if the gain and timing get updated
  _timing = timing;

  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  switch(gain)
  {
    case 0:
      tsl.setGain(TSL2591_GAIN_LOW); // 1x gain (bright light)
      break;
    case 1:
      tsl.setGain(TSL2591_GAIN_MED); // 25x gain
      break;
    case 2:
      tsl.setGain(TSL2591_GAIN_HIGH); // 428x gain
      break;
    default:
      tsl.setGain(TSL2591_GAIN_MAX); // 9876x gain
  }
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  switch(timing)
  {
    case 0:
      tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS); //shortest integration time (bright light)
      break;
    case 1:
      tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
      break;
    case 2:
      tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
      break;
    case 3:
      tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
      break;
    case 4:
      tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
      break;
    default:
      tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS); //longest integration time (dim light)
  }

  /* TODO: Confirm changes by comparing the get_value to the set_value */
  return true; //return true for now
}

/* Calculates the current visible Lux value and then sends the sensor back to sleep */
float TSL2591::luxRead()
{
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity();
  
  uint16_t full = lum & 0xFFFF;
  uint16_t ir = lum >> 16;
  
  /* "full" values 0 and 65535 are invalid and thus gain should be changed */

  return tsl.calculateLux(full, ir);
}

