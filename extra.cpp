/* This contains all the extra utility functions for the lightmeter */

#include "extra.h" // include my header file

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
   */
  for(byte x = 0; x < flashes; x++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
  extra::sleep(800); //800ms sleep delay after a LED signal to mark a distinct end to the signal
}

/* SLEEP MODE HELPER TO EXPAND ON SLEEPYDOG'S MAX 8000ms SLEEP */
void extra::sleep(uint32_t sTime)
{
  while(sTime > 0)
  {
    sTime -= Watchdog.sleep(sTime);
  }
}

