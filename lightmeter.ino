/*
  DIY Kuffner-Sternwarte Lightmeter

  TODO:
  * Check filesize and create another if size > filesystem max file size.
  * Sleep Mode/Standby between data logging.
  * LED feedback for errors and stuff.
  
*/

#include <SPI.h> //SD Card
#include <SD.h> //SD Card
#include <ArduinoJson.h> //For JSON data formatting
#include "rtc.h"
#include "lightsensor.h"

TSL2591 lightsensor = TSL2591(0, 0);
RTC_DS3231 rtc = RTC_DS3231();

const int SD_PIN = 4; //pin connected to the chip select line of the SD card
const String FILENAME = "light_data.json"; //filename for the data file

/* ARDUINO SETUP FUNCTION */
void setup() {

  /* WAIT FOR SERIAL; DEBUGGING ONLY */
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  /* INIT TSL2591 IF PRESENT */
  Serial.print("Initializing the TSL2591.");
  if (!lightsensor.begin()) {
    Serial.println("No lightsensor found! Please restart!");
    while(true); //don't do anything anymore
  }
  Serial.print("TSL2591 initialized.");

  /* INIT SD CARD IF PRESENT */
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_PIN)) {
    Serial.println("Card failed, or not present");
    while(true); //don't do anything anymore
  }
  Serial.println("SD Card initialized.");

  // Display some basic information on this sensor
  lightsensor.displaySensorDetails();
}

/* MAIN LOOP */
void loop() {
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

  // open the data file, only one file at a time!
  File dataFile = SD.open(FILENAME, FILE_WRITE);
  
  //if the file is available, write the data to it
  if (dataFile) {
    data.printTo(dataFile);
    dataFile.close();
  }
  else //if the file is not available, print an error message.
  {
    Serial.println("Error opening " + FILENAME + "!");
  }
}

/* ERROR LED FUNCTION */
void error_led(unsigned int errnum = 0)
{
  //nothing yet
}

