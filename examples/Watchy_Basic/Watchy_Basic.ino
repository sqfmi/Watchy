/*
 * Watchy - Basic Example 
 * Sets the RTC time if not set, goes to sleep and wakes up every minute to update the time on the display
 */
 
#include <DS3232RTC.h>
#include <GxEPD.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include "GxGDEH0154D67.h"
#include "DSEG7_Classic_Bold_48.h"

#define RTC_PIN GPIO_NUM_33

DS3232RTC RTC(false);
GxIO_Class io(SPI, /*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16); // arbitrary selection of 17, 16
GxEPD_Class display(io, /*RST=*/ 16, /*BUSY=*/ 4); // arbitrary selection of (16), 4

void setup()
{
    detect_wakeup_reason();
    delay(100);
    esp_sleep_enable_ext0_wakeup(RTC_PIN, 0); //enable deep sleep wake on RTC interrupt
    esp_deep_sleep_start();
}

void loop(){}

void updateTime(bool fullRefresh)
{
    RTC.begin();
    if(RTC.oscStopped(false)){ //check if RTC has been stopped
      RTC.squareWave(SQWAVE_NONE); //disable square wave output
      RTC.set(compileTime()); //set RTC time to compile time
      RTC.setAlarm(ALM2_EVERY_MINUTE, 0, 0, 0, 1); //set alarm to every minute
      RTC.alarmInterrupt(ALARM_2, true); //enable alarm interrupt
    }
    RTC.alarm(ALARM_2); //resets the alarm flag in the RTC
    tmElements_t currentTime;
    RTC.read(currentTime);

    display.init();
    display.fillScreen(GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    display.setFont(&DSEG7_Classic_Bold_48);
    display.setCursor(15, 120);

    if(currentTime.Hour < 10){
      display.print('0');
    }
    display.print(currentTime.Hour);
    display.print(':');
    if(currentTime.Minute < 10){
      display.print('0');
    }    
    display.print(currentTime.Minute);
    
    if(fullRefresh){
      display.update();
    }else{
      display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, true); 
    }
    
    display.deepSleep();
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

void detect_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0: updateTime(false); break; //RTC Alarm
    default: updateTime(true); //Reset
  }
}
