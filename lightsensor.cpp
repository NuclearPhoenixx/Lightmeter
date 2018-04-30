/* This contains all the functions for the TSL2591 light sensor */

#include "lightsensor.h" // include my header file

#include <Adafruit_TSL2591.h> //TSL2591 lib
#include <Adafruit_Sensor.h> //for displaySensorDetails()

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)

/* DEF CONSTRUCTOR */
TSL2591::TSL2591(unsigned int gain, unsigned int timing)
{
  _gain = gain;
  _timing = timing;
}

/* SETUP FUNCTION */
bool TSL2591::begin()
{
  tsl.begin();
  TSL2591::configureSensor(_gain, _timing); //first config with the vars from the constructor

  /* TODO: Check if begin was successfull */
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
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" lux"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution, 4); Serial.println(F(" lux"));  
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
}

/* Configure gain and integration time for the TSL2591 */
void TSL2591::configureSensor(unsigned int gain, unsigned int timing)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  switch(gain)
  {
    case 0: tsl.setGain(TSL2591_GAIN_LOW); // 1x gain (bright light)
    case 1: tsl.setGain(TSL2591_GAIN_MED); // 25x gain
    default: tsl.setGain(TSL2591_GAIN_HIGH); // 428x gain
  }
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  switch(timing)
  {
    case 0: tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS); //shortest integration time (bright light)
    case 1: tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
    case 2: tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
    case 3: tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
    case 4: tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
    default: tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS); //longest integration time (dim light)
  }

  /* TODO: Confirm changes by comparing the get_value to the set_value */
}

/* Perform a basic read on visible, full spectrum or infrared light (returns raw 16-bit ADC values) */
double TSL2591::simpleRead(unsigned int spectrum)
{
  // Simple data read example. Just read the infrared, fullspecrtrum diode 
  // or 'visible' (difference between the two) channels.
  // This can take 100-600 milliseconds! Uncomment whichever of the following you want to read
  uint16_t x;
  
  switch(spectrum)
  {
    case 0: x = tsl.getLuminosity(TSL2591_VISIBLE); //perform measurement of visible spectrum
    case 1: x = tsl.getLuminosity(TSL2591_INFRARED); //perform measurement of infrared spectrum
    default: x = tsl.getLuminosity(TSL2591_FULLSPECTRUM); //default measurement is full spectrum
  }
  
  return (x, DEC);
}

/* Show how to read IR and Full Spectrum at once and convert to lux */
double TSL2591::advancedRead(unsigned int spectrum)
{
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity();

  uint16_t ir = lum >> 16;
  uint16_t full = lum & 0xFFFF;
  
  switch(spectrum)
  {
    case 0: return ir; //infrared
    case 1: return full; //full spectrum
    case 3: return full - ir; //visible spectrum
    default: return tsl.calculateLux(full, ir); //lux value
  }
}

/* Performs a read using the Adafruit Unified Sensor API. */
double TSL2591::unifiedSensorAPIRead()
{
  /* Get a new sensor event */ 
  sensors_event_t event;
  tsl.getEvent(&event);
 
  /* Return the results (light is measured in lux) */
  if ((event.light == 0) | (event.light > 4294966000.0) | (event.light <-4294966000.0))
  {
    /* If event.light = 0 lux the sensor is probably saturated */
    /* and no reliable data could be generated! */
    /* if event.light is +/- 4294967040 there was a float over/underflow */
    return;
  }
  else
  {
    return event.light; //return lux value
  }
}

