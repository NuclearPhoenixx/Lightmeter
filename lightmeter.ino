/* DIY Kuffner-Sternwarte Lightmeter */

#include "rtc.h"
#include "lightsensor.h"

lightsensor lightsensor(3);
rtc rtc(6);

/* SETUP ONLY */
void setup() {
  /* Display some basic information on this sensor */
  lightsensor.displaySensorDetails();
  
  /* Configure the sensor */
  lightsensor.configureSensor();

}

/* MAIN LOOP */
void loop() { 
  
  lightsensor.advancedRead();

  Serial.println(rtc.get_unix_time());
  
}
