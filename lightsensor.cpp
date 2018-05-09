/* This contains all the functions for the TSL2591 light sensor */

#include "lightsensor.h" // include my header file

#include <Adafruit_TSL2591.h> //TSL2591 lib

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)

/* DEF CONSTRUCTOR */
TSL2591::TSL2591(char gain, char timing)
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

  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  switch(gain)
  {
    case 0:
      tsl.setGain(TSL2591_GAIN_LOW); // 1x gain (bright light)
      break;
    case 1:
      tsl.setGain(TSL2591_GAIN_MED); // 25x gain
      break;
    default:
      tsl.setGain(TSL2591_GAIN_HIGH); // 428x gain
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

/* Perform a basic read on visible, full spectrum or infrared light (returns raw 16-bit ADC values) */
uint16_t TSL2591::simpleRead(char spectrum)
{
  // Simple data read example. Just read the infrared, fullspecrtrum diode 
  // or 'visible' (difference between the two) channels.
  // This can take 100-600 milliseconds! Uncomment whichever of the following you want to read
  uint16_t ir = tsl.getLuminosity(TSL2591_INFRARED);
  uint16_t full = tsl.getLuminosity(TSL2591_FULLSPECTRUM);
  
  switch(spectrum)
  {
    case 0:
      return tsl.getLuminosity(TSL2591_VISIBLE); //perform measurement of visible spectrum
      break;
    case 1:
      return ir; //perform measurement of infrared spectrum
      break;
    default:
      return full; //default measurement is full spectrum
  }
}

/* Show how to read IR and Full Spectrum at once and convert to lux */
uint16_t TSL2591::advancedRead(char spectrum)
{
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity();

  uint16_t ir = lum >> 16;
  uint16_t full = lum & 0xFFFF;
  
  switch(spectrum)
  {
    case 0:
      return full - ir; //visible spectrum
      break;
    case 1:
      return ir; //infrared
      break;
    default:
      return full; //full spectrum
  }
}

/* Calculates the current visible Lux value and then sends the sensor back to sleep */
float TSL2591::luxRead()
{
  tsl.enable(); //enable the TSL2591
  float reading = tsl.calculateLux(tsl.getLuminosity(TSL2591_FULLSPECTRUM), tsl.getLuminosity(TSL2591_INFRARED));
  tsl.disable(); //set the TSL2591 to power down mode
  return reading;
}

