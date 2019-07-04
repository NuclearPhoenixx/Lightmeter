/*
 * All-in-One Arduino-compatible Luxmeter.
 * https://github.com/Phoenix1747/Lightmeter
 * 
 * THIS PROJECT IS LICENSED UNDER GPL-3.0, FEEL FREE TO USE IT
 * ACCORDING TO THE LICENSE. HAVE FUN!
 * 
 * Please note that this is still work in progress and is probably really unstable.
*/
#include <SD.h>
#include <ArduinoJson.h>
#include <RTClib.h>
#include <Adafruit_TSL2591.h>

#include "Support.h" //All the extra needed functionality.

#define SD_CD 7 //Pin connected to the uSD card detect pin.
#define SD_CS 4 //Pin connected to the uSD card CS pin.
#define TSL2591_LUX_DF 650.0F //Lux cooefficient for calibration.

/* == SETTINGS == */
const String FILE_NAME = "DATA"; //Filename for the data file; 8 chars or less (FAT32 limit).
const String FILE_EXTENSION = "TXT"; //File extension for the data file; 3 chars or less (FAT32 limit).
const uint32_t MAX_FILESIZE = 4000000000; //Max filesize in bytes; defaults 4GB (FAT32 size limit).
const uint16_t M_INTERVAL = 5000; //Time between measurements [ms].
/* == == */

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
RTC_DS3231 rtc;

File dataFile; //Global variable that will hold the data file.
byte fileNum = 0; //Global number of files written.
String filePath; //This will hold the file path globally.

/* == MCU SETUP == */
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); //Set builtin LED to output.
  pinMode(SD_CD, INPUT_PULLUP); //Setup the uSD card detect pin.
  
  if(!tsl.begin() || !SD.begin(SD_CS) || !rtc.begin() || rtc.lostPower()) //Initialize all the parts.
  { while(1){ support::ledFlash(); } }

  tslSetup(); //Configure the TSL2591.
  
  filePath = FILE_NAME + "." + FILE_EXTENSION; //Setup the file path for the first time.
  dataFile = SD.open(filePath, FILE_WRITE); //Open that one data file.
  
  support::ledFlash(); //Test LED at startup and let sensor warm up.
}

/* == MAIN LOOP == */
void loop()
{
  if(digitalRead(SD_CD)) //Check for physically inserted uSD card.
  {
    support::ledFlash(); //Flash to indicate a problem.
    return;
  }
  
  StaticJsonDocument<45> jsonDoc; //New static JSON doc and allocate 38 bytes (cover "worst" case).
  JsonObject data = jsonDoc.to<JsonObject>(); //Create JSON object that will contain all the data.
  
  data[F("unixtime")] = rtc.now().unixtime(); //Input current RTC unixtime.
  data[F("lux")] = measureLux(); //Input measured lux value.
  data[F("temp")] = rtc.getTemperature(); //Input measured (RTC) temperature in °C for additional info.

  auto jsonSize = measureJson(data); //Compute JSON size in byte.
  uint32_t fileSize = dataFile.size() + jsonSize; //Size of the updated save file in byte.
    
  //Check if the file size is bigger than the max set size; iterate until a valid file is found.
  while(fileSize > MAX_FILESIZE)
  {
    dataFile.close(); //Close old file.
    
    fileNum++; //Add 1 to the file number.
    filePath = FILE_NAME + fileNum + "." + FILE_EXTENSION; //Update filename to include the file number.
    dataFile = SD.open(filePath, FILE_WRITE);
    
    fileSize = dataFile.size() + jsonSize;
  }
    
  //If the data file is available write to it.
  if(dataFile)
  {
    serializeJson(data, dataFile); //Append the JSON data.
    dataFile.flush(); //Physically save the data, needs up to 3x the power though.
  }
  else //If no file is available flash an error.
  {
    support::ledFlash();
  }
  
  support::sleep(M_INTERVAL); //Sleep until the next measurement.
}

/* == TSL2591 SETUP == */
void tslSetup()
{
  tsl.setGain(TSL2591_GAIN_LOW); //TSL2591_GAIN_LOW, TSL2591_GAIN_MED, TSL2591_GAIN_HIGH, TSL2591_GAIN_MAX
  tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS); //TSL2591_INTEGRATIONTIME_100MS, TSL2591_INTEGRATIONTIME_200MS, TSL2591_INTEGRATIONTIME_300MS, TSL2591_INTEGRATIONTIME_400MS, TSL2591_INTEGRATIONTIME_500MS, TSL2591_INTEGRATIONTIME_600MS
}

/* == TSL LUX COMPUTING == */
uint16_t _minBit = 3277; //Min and max lightsensor returns. Default +-5%.
uint16_t _maxBit = 62258;

float measureLux()
{
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full = 0;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  
  while(full > _maxBit || full < _minBit) //Check near overflow or 0.
  {
    autoRange(full);
    delay(600); //Sleep 600 ms to chill the sensor.
    
    lum = tsl.getFullLuminosity();
    ir = lum >> 16;
    full = lum & 0xFFFF;
  }
  
  return tsl.calculateLux(full, ir);
}

/* == AUTO SCALE LUX RANGE == */
tsl2591IntegrationTime_t timings[6] = {TSL2591_INTEGRATIONTIME_100MS, TSL2591_INTEGRATIONTIME_200MS, TSL2591_INTEGRATIONTIME_300MS, TSL2591_INTEGRATIONTIME_400MS, TSL2591_INTEGRATIONTIME_500MS, TSL2591_INTEGRATIONTIME_600MS};
tsl2591Gain_t gains[4] = {TSL2591_GAIN_LOW, TSL2591_GAIN_MED, TSL2591_GAIN_HIGH, TSL2591_GAIN_MAX};
byte _timing = 0; //0-5
byte _gain = 0; //0-3

void autoRange(uint16_t full) //Suggestions for this part greatly appreciated, it's *ugly*!
{
  if(full < _minBit) //Compute close to 0.
  {
    if(_timing < 5)
    {
      _timing++; //Increase Timing if possible.
    }
    else if(_gain < 3)
    {
      _gain++; //Increase Gain if Timing is already max.
    }
    else
    {
      support::ledFlash(); //Something's not right here, ABORT.
    }
  }
  else if(full > _maxBit) //Compute close to overflow.
  {
    if(_gain > 0)
    {
      _gain--; //Decrease Gain if possible.
    }
    else if(_timing > 0)
    {
      _timing--; //Decrease Timing if Gain is already min.
    }
    else
    {
      support::ledFlash(); //Something's not right here, ABORT.
    }
  }

  tsl.setGain(gains[_gain]); //Update lightsensor settings.
  tsl.setTiming(timings[_timing]);
}
