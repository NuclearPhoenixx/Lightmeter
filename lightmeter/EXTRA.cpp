/* This contains all the extra utility functions for the lightmeter */

#include "EXTRA.h" // include my header file
#include <Adafruit_SleepyDog.h> //For power down sleep mode

/* SIGNAL LED FUNCTION FOR ERRORS AND USER INTERFACE  */
void extra::signal_led(byte flashes)
{
  /* ERROR/SIGNAL FLASHES:
   *  1: NO LIGHTSENSOR
   *  2: NO SD CARD
   *  3: FILE WRITE ERROR
   *  4: SD FILESYSTEM ERROR
   *  5: NO RTC
   *  6: OTHER MISC ERROR
   *  7: RTC TIME NOT SET
   *  8: NO FRAM CHIP FOUND
   */
  extra::sleep(800); //800ms sleep delay before LED status signaling
  
  for(byte x = 0; x < flashes; x++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    extra::sleep(200);
    digitalWrite(LED_BUILTIN, LOW);
    extra::sleep(200);
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

