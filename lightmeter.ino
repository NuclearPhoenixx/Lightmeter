/*
  DIY Kuffner-Sternwarte Lightmeter

  TODO:
  * Check if SD Card space is full.
  * Sleep Mode/Standby between data logging.
  * Detect when the clock battery is low.
  * Use C datatypes.
  * Fix negative lux values.
*/

#include <SPI.h> //SD Card
#include <SD.h> //SD Card
#include <ArduinoJson.h> //For JSON data formatting
#include "rtc.h"
#include "lightsensor.h"

#define _MAJORV 1 //major firmware version
#define _MINORV 2 //minor firmware version

/* BEGIN USER CONFIG */
const unsigned int SD_PIN = 4; //pin connected to the chip select line of the SD card
const String FILE_NAME = "data"; //filename for the data file; 8 chars or less!
const String FILE_EXTENSION = ".txt"; //file extension for the data file; 3 chars or less!
const unsigned long MAX_FILESIZE = 500000000; //max filesize in byte, here it's 500MB (NOTE FAT32 SIZE LIMIT!)
const unsigned int M_INTERVAL = 1000; //measurement interval for data logging, in ms
/* END USER CONFIG */

TSL2591 lightsensor = TSL2591(0, 0); //create the objects for my classes
DS3231 rtc = DS3231();

File dataFile; //global variable that will hold the data file
unsigned int fileNum = 0; //global number of file
String filePath; //this will hold the file path globally

/* SIGNAL LED FUNCTION FOR ERRORS AND USER INTERFACE  */
void signal_led(unsigned int num = 0)
{
  unsigned int flashes;
  
  switch(num)
  {
    case 0: //OK
      flashes = 1;
      break;
    case 1: //NO LIGHTSENSOR
      flashes = 2;
      break;
    case 2: //NO SD CARD
      flashes = 3;
      break;
    case 3: //FILE WRITE ERROR
      flashes = 4;
      break;
    case 4: //SD FILESYSTEM ERROR
      flashes = 5;
      break;
    case 5: //NO RTC
      flashes = 6;
      break;
    default: //OTHER MISC ERROR
      flashes = 6;
  }

  for(unsigned int x = 0; x < flashes; x++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
  delay(800); //800ms delay after a LED signal to mark a distinct end to the flash
}

/* ARDUINO SETUP FUNCTION */
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); //set builtin LED to output
  pinMode(13, OUTPUT); //sd card led

  Serial.begin(9600); //begin serial (for debugging and rtc set)

  // FIRMWARE LED FLASH
  for(unsigned int x = 0; x < _MAJORV; x++) //flash major version
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
  
  delay(400); //400ms delay between the stages
  
  for(unsigned int x = 0; x < _MINORV; x++) //flash minor version
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
  
  delay(1500); //1.5s delay between coming errors and firmware flash

  /* Display some basic information on this sensor
  lightsensor.displaySensorDetails(); */

  // INIT TSL2591 IF PRESENT
  if(!lightsensor.begin())
  {
    while(1)
    {
      signal_led(1);
    }
  }
  
  signal_led(0); //successfull

  // INIT SD CARD IF PRESENT
  if(!SD.begin(SD_PIN))
  {
    while(1)
    {
      signal_led(2);
    }
  }

  signal_led(0); //successfull

  //INIT RTC
  if(!rtc.begin())
  {
    while(1)
    {
      signal_led(5);
    }
  }
  
  signal_led(0); //successfull

  //SET RTC TIME IF NO TIME SET
  if(rtc.lostPower())
  {
    rtc.setTime();
  }

  //update filePath to point to file_name.file_extenion.
  filePath = FILE_NAME + FILE_EXTENSION;
  dataFile = SD.open(filePath, FILE_WRITE); // open the data file, only one file at a time!
}

/* MAIN LOOP */
void loop()
{
  // new dynamic json buffer, let's just assume 20 bytes for now
  DynamicJsonBuffer jsonBuffer(20);
  
  // create new json object that will contain all the logged data
  JsonObject& data = jsonBuffer.createObject();
  
  data["unixtime"] = rtc.unixtime(); //input rtc time
  data["lux"] = lightsensor.luxRead(); //input lux value

  //to-be/future size of file with the new data in bytes
  unsigned long future_size = dataFile.size() + data.measureLength();
  
  //check if future_size is bigger than specified max size, if yes, write to new file
  if(future_size > MAX_FILESIZE)
  {
    fileNum++; //add 1 to the file number
    filePath = FILE_NAME + fileNum + FILE_EXTENSION; //update filename to include the file number
    dataFile = SD.open(filePath, FILE_WRITE);
  }

  //if the data file is available, write the data to it
  if(dataFile)
  {
    data.printTo(dataFile); //compactly append to the file
    dataFile.flush(); //save the data to the file, needs up to 3x the power
  }
  else //if the file is not available, flash an error
  {
    signal_led(3);
  }
  delay(M_INTERVAL); //sleep for the set time
}

