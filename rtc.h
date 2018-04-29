/* This contains all the functions for the DS3231 real time clock */

#ifndef rtc_h
#define rtc_h

class rtc
{

  public:
    rtc(int addr); //setup function
  
	  unsigned int get_unix_time();
   
  private:
    int _addr;

};

#endif
