/* This contains all the support functionality for the lightmeter */
#include "Support.h"

#include <Adafruit_SleepyDog.h> //For power down sleep mode

/* == LED STATUS FLASH == */
void support::ledFlash()
{
  delay(800); //Delay before LED flash

  for (byte i = 0; i < 3; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
}

/* == ADVANCED SLEEP MODE == */
void support::sleep(uint32_t sleep_time)
{
  while (sleep_time > 0)
  {
    sleep_time -= Watchdog.sleep(sleep_time);
  }
}
