/*
 * Watchy - DOS Watchface Example 
 * Sets the RTC time if not set, goes to sleep and wakes up every minute to update the time on the display
 */

#ifndef ESP32
#error Please select ESP32 Wrover Module under Tools > Board
#endif

#include <DS3232RTC.h>
#include <GxEPD2_BW.h>
#include "Px437_IBM_BIOS5pt7b.h"

#define RTC_PIN GPIO_NUM_33

DS3232RTC RTC(false);
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEH0154D67

void setup()
{
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0: updateTime(false); break; //RTC Alarm Interrupt
    default: updateTime(true); //Hard Reset
  }
  esp_sleep_enable_ext0_wakeup(RTC_PIN, 0); //enable deep sleep wake on RTC interrupt
  esp_deep_sleep_start();
}

void loop(){}

void updateTime(bool reset)
{
  RTC.begin();
  if(reset){
    RTC.squareWave(SQWAVE_NONE); //disable square wave output
    RTC.set(compileTime()); //set RTC time to compile time
    RTC.setAlarm(ALM2_EVERY_MINUTE, 0, 0, 0, 0);
    RTC.alarmInterrupt(ALARM_2, true); //enable alarm interrupt
  }
  RTC.alarm(ALARM_2); //resets the alarm flag in the RTC
  tmElements_t currentTime;
  RTC.read(currentTime);

  char time[6];
  time[0] = '0' + ((currentTime.Hour/10)%10);
  time[1] = '0' + (currentTime.Hour%10); 
  time[2] = ':';
  time[3] = '0' + ((currentTime.Minute/10)%10);
  time[4] = '0' + (currentTime.Minute%10); 
  time[5] = 0;

  display.init(0, reset); //_initial_refresh to false to prevent full update on init
  display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  display.setTextColor(GxEPD_WHITE);
  display.setFont(&Px437_IBM_BIOS5pt7b);
  display.setCursor(0, 24);
  display.println("WATCHY-DOS 1.1.4");
  display.println("Copyright (c) 2020");
  display.println(" ");
  display.print("AUTOEXEC BAT ");
  display.println(time);
  display.print("COMMAND  COM ");
  display.println(time);
  display.print("CONFIG   SYS ");
  display.println(time);
  display.print("ESPTOOL  PY  ");
  display.println(time);
  display.println(" ");
  display.println("  4 files 563 bytes");
  display.println("  2048 bytes free");
  display.println(" ");
  display.println("<C:\\>esptool");
/*
  if(currentTime.Hour < 10){
    display.print('0');
  }
  display.print(currentTime.Hour);
  display.print(':');
  if(currentTime.Minute < 10){
    display.print('0');
  }    
  display.print(currentTime.Minute);*/
  display.display(true); //partial refresh
  display.hibernate();
}

time_t compileTime()
{
    const time_t FUDGE(10);    //fudge factor to allow for upload time, etc. (seconds, YMMV)
    const char *compDate = __DATE__, *compTime = __TIME__, *months = "JanFebMarAprMayJunJulAugSepOctNovDec";
    char compMon[3], *m;

    strncpy(compMon, compDate, 3);
    compMon[3] = '\0';
    m = strstr(months, compMon);

    tmElements_t tm;
    tm.Month = ((m - months) / 3 + 1);
    tm.Day = atoi(compDate + 4);
    tm.Year = atoi(compDate + 7) - 1970;
    tm.Hour = atoi(compTime);
    tm.Minute = atoi(compTime + 3);
    tm.Second = atoi(compTime + 6);

    time_t t = makeTime(tm);
    return t + FUDGE;        //add fudge factor to allow for compile time
}
