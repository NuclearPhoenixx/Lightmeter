/* This contains all the support functionality for the lightmeter */
#include "Support.h"

#include <Adafruit_SleepyDog.h> //For power down sleep mode

/* == LED STATUS FLASH == */
void support::ledFlash()
{
  delay(600); //Delay before LED flash
  
  for(byte i = 0; i < 3; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(180);
    digitalWrite(LED_BUILTIN, LOW);
    delay(180);
  }
}

/* == ADVANCED SLEEP MODE == */
void support::sleep(uint32_t sleepTime)
{
  while(sleepTime > 0)
  {
    sleepTime -= Watchdog.sleep(sleepTime);
  }
}
