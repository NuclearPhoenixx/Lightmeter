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

#include "Support.h" //All the extra needed functionality

#define SD_CD 7 //Pin connected to the uSD card detect pin
#define SD_CS 4 //Pin connected to the uSD card CS pin
#define LED_BUILTIN 2 //Pin for the status LED
#define TSL2591_LUX_DF 650.0F //Lux cooefficient for calibration

/* == SETTINGS == */
const String FILE_NAME = "data"; //Filename for the data file; 8 chars or less (FAT32 limit)
const String FILE_EXTENSION = "txt"; //File extension for the data file; 3 chars or less (FAT32 limit)
const uint32_t MAX_FILESIZE = 4000000000; //Max filesize in bytes; defaults 4GB (FAT32 size limit)

const uint16_t M_INTERVAL = 5000; //Time between measurements [ms]
/* == == */

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
RTC_DS3231 rtc;

File dataFile; //Global variable that will hold the data file
byte fileNum = 0; //Global number of files written
String filePath; //This will hold the file path globally

/* == MCU SETUP == */
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); //Set builtin LED to output
  pinMode(SD_CD, INPUT_PULLUP); //Setup the uSD card detect pin
  
  /* //Display some basic information on this sensor
  tsl.displaySensorDetails(); */
  
  if(!tsl.begin() || !SD.begin(SD_CS) || !rtc.begin() || rtc.lostPower()) //Initialize all the parts
  { while(1){ support::ledFlash(); } }

  tslSetup(); //Configure the TSL2591
  
  filePath = FILE_NAME + "." + FILE_EXTENSION; //Setup the file path for the first time
  dataFile = SD.open(filePath, FILE_WRITE); //Open that one data file
  
  support::ledFlash(); //Test LED at startup
  
  //Serial.begin(9600); //DEBUGGING SERIAL
}

/* == MAIN LOOP == */
void loop()
{
  if(digitalRead(SD_CD)) //Check for physically inserted uSD card
  {
    support::ledFlash(); //Flash to indicate a problem
    return;
  }
  
  StaticJsonDocument<38> jsonDoc; //New static JSON doc and allocate 38 bytes (worst case)
  JsonObject data = jsonDoc.to<JsonObject>(); //Create JSON object that will contain all the data
  
  data[F("unixtime")] = rtc.now().unixtime(); //Input current RTC unixtime
  data[F("lux")] = measureLux(); //Input measured lux value

  auto jsonSize = measureJson(data); //Compute JSON size in byte
  uint32_t fileSize = dataFile.size() + jsonSize; //Size of the updated save file in byte
    
  //Check if the file size is bigger than the max set size; iterate until a valid file is found
  while(fileSize > MAX_FILESIZE)
  {
    dataFile.close(); //close old file
    
    fileNum++; //add 1 to the file number
    filePath = FILE_NAME + fileNum + "." + FILE_EXTENSION; //update filename to include the file number
    dataFile = SD.open(filePath, FILE_WRITE);
    
    fileSize = dataFile.size() + jsonSize;
  }
    
  //If the data file is available write to it
  if(dataFile)
  {
    serializeJson(data, dataFile); //Append the JSON data
    dataFile.flush(); //Physically save the data, needs up to 3x the power though
  }
  else //If no file is available flash an error
  {
    support::ledFlash();
  }
  
  support::sleep(M_INTERVAL); //Sleep until the next measurement
}

/* == TSL2591 SETUP == */
void tslSetup()
{
  tsl.setGain(TSL2591_GAIN_MED); //TSL2591_GAIN_LOW, TSL2591_GAIN_MED, TSL2591_GAIN_HIGH, TSL2591_GAIN_MAX
  tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS); //TSL2591_INTEGRATIONTIME_100MS, TSL2591_INTEGRATIONTIME_200MS, TSL2591_INTEGRATIONTIME_300MS, TSL2591_INTEGRATIONTIME_400MS, TSL2591_INTEGRATIONTIME_500MS, TSL2591_INTEGRATIONTIME_600MS
}

/* == TSL LUX COMPUTING == */
float measureLux()
{
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  
  ir = lum >> 16;
  full = lum & 0xFFFF;
  
  //Serial.println(lum,6);
  return tsl.calculateLux(full, ir);
}
