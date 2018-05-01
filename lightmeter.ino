/*
  DIY Kuffner-Sternwarte Lightmeter

  TODO:
  * Check if SD Card is full.
  * Sleep Mode/Standby between data logging.
  * Detect when the clock battery is low.
  * Detect when my own lipo battery is low.
  * Use C datatypes?
  
*/

#include <SPI.h> //SD Card
#include <SD.h> //SD Card
#include <ArduinoJson.h> //For JSON data formatting
#include "rtc.h"
#include "lightsensor.h"

#define _VERSION 1.1 //firmware version

/* USER CONFIG */
const unsigned int SD_PIN = 4; //pin connected to the chip select line of the SD card
const String FILE_NAME = "data"; //filename for the data file
const String FILE_EXTENSION = ".json"; //file extension for the data file
const unsigned int MAX_FILESIZE = 500000000; //max filesize in byte, here it's 500MB (NOTE FAT32 SIZE LIMIT!)
/* USER CONFIG */


TSL2591 lightsensor = TSL2591(0, 0); //create the objects for my classes
RTC_DS3231 rtc = RTC_DS3231();

File dataFile; //global variable that will hold the data file
unsigned int fileNum = 0; //global number of file
String filePath; //this will hold the file path globally

/* SIGNAL LED FUNCTION FOR ERRORS AND USER INTERFACE  */
void signal_led(unsigned int num = 0)
{
  unsigned int flashes;
  unsigned int interval = 300;
  
  switch(num)
  {
    case 0: //OK
      flashes = 1;
      interval = 200;
    case 1: //NO LIGHTSENSOR
      flashes = 2;
    case 2: //NO SD CARD
      flashes = 3;
    case 3: //FILE ERROR
      flashes = 4;
    case 4: //SD FILESYSTEM ERROR
      flashes = 5;
    default: //OTHER MISC ERROR
      flashes = 6;
  }

  for(unsigned int x = 0; x < flashes; x++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(interval);
    digitalWrite(LED_BUILTIN, LOW);
  }
  delay(800); //800ms delay after a LED signal to mark a distinct end to the flash
}

/* ARDUINO SETUP FUNCTION */
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); //set builtin LED to output

  // FIRMWARE LED FLASH
  unsigned int major = (int)_VERSION;
  unsigned int minor = _VERSION - (float)major * 10.0;
  
  for(unsigned int x = 0; x < major; x++) //flash major version
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  delay(500); //500ms delay between the stages
  
  for(unsigned int x = 0; x < minor; x++) //flash minor version
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
  }

  /* WAIT FOR SERIAL; DEBUGGING ONLY
  Serial.begin(9600);
  while (!Serial) { //wait for serial port to connect. Needed for native USB port only
    digitalWrite(LED_BUILTIN, HIGH);
    delay(800);
    digitalWrite(LED_BUILTIN, LOW);
  } */

  // INIT TSL2591 IF PRESENT
  if (!lightsensor.begin())
  {
    while(1)
    {
      signal_led(1); //don't do anything anymore, only drop error
    }
  }
  
  signal_led(0); //successfull

  // INIT SD CARD IF PRESENT
  if (!SD.begin(SD_PIN))
  {
    while(1)
    {
      signal_led(2); //don't do anything anymore, only drop error
    }
  }

  signal_led(0); //successfull

  //update filePath to point to file_name.file_extenion.
  filePath = FILE_NAME + FILE_EXTENSION;
  dataFile = SD.open(filePath, FILE_WRITE); // open the data file, only one file at a time!

  /* Display some basic information on this sensor
  lightsensor.displaySensorDetails(); */
}

/* MAIN LOOP */
void loop()
{
  //FOR LIGHTSENSOR TESTING
  unsigned int a = lightsensor.simpleRead();
  unsigned int b = lightsensor.advancedRead();
  unsigned int c = lightsensor.unifiedSensorAPIRead();
  
  // new dynamic json buffer, let's just assume 20 bytes for now
  DynamicJsonBuffer jsonBuffer(20);
  
  // create new json object that will contain all the logged data
  JsonObject& data = jsonBuffer.createObject();
  
  data["unixtime"] = rtc.get_unix_time();
  data["lux"] = a;

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
}

