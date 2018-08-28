/* This contains all the extra utility functions for the lightmeter */

#include "EXTRA.h" // include my header file
#include <Adafruit_SleepyDog.h> //For power down sleep mode

#define LED_BUILTIN 2 //pin for the LED

/* FLASH THE LED 3 TIMES TO INDICATE A PROBLEM  */
void extra::led_flash()
{
  delay(800); //800ms sleep delay before LED flash
  
  for(byte x = 0; x < 3; x++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
}

/* SLEEP MODE HELPER TO EXPAND ON SLEEPYDOG'S MAX 8000ms SLEEP */
void extra::sleep(uint32_t sTime)
{
  while(sTime > 0)
  {
    sTime -= Watchdog.sleep(sTime);
  }
}

