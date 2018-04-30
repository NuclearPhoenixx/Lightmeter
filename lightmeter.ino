/* DIY Kuffner-Sternwarte Lightmeter */

#include "rtc.h"
#include "lightsensor.h"

TSL2591 lightsensor = TSL2591(0, 0);
RTC_DS3231 rtc = RTC_DS3231();

/* SETUP ONLY */
void setup() {
  
  lightsensor.begin(); //init the lightsensor lib
  
  lightsensor.displaySensorDetails(); // Display some basic information on this sensor
  //lightsensor.configureSensor(); // Configure the sensor; this is already done in begin()

}

/* MAIN LOOP */
void loop() { 
  
  int a = lightsensor.simpleRead();
  int b = lightsensor.advancedRead();
  int c = lightsensor.unifiedSensorAPIRead();
  int d = rtc.get_unix_time();
  
}
