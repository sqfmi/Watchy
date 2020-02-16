/*
 * Watchy - Analog Example 
 * Sets the RTC time if not set, goes to sleep and wakes up every minute to update the time on the display
 */

#include <WiFi.h>
#include <DS3232RTC.h>
#include <GxEPD.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include "GxGDEH0154D67.h"

#define RTC_PIN GPIO_NUM_33
#define ORIGIN_X GxGDEH0154D67_WIDTH/2
#define ORIGIN_Y GxGDEH0154D67_HEIGHT/2
#define HOUR_HAND_LENGTH 50
#define HOUR_HAND_WIDTH 11
#define MINUTE_HAND_LENGTH 90
#define MINUTE_HAND_WIDTH 7

DS3232RTC RTC(false);
GxIO_Class io(SPI, /*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16); // arbitrary selection of 17, 16
GxEPD_Class display(io, /*RST=*/ 16, /*BUSY=*/ 4); // arbitrary selection of (16), 4

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

  display.init();
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  setHands(currentTime);

  display.update();
  display.deepSleep();
}

void setHands(tmElements_t time){
  //hour
  float hour_radians = ((time.Hour % 12)/12.0)*2*M_PI; //radians
  int hour_x1 = sinf(hour_radians)*HOUR_HAND_LENGTH;
  int hour_y1 = cosf(hour_radians)*HOUR_HAND_LENGTH;
  fillRectAngle(ORIGIN_X, ORIGIN_Y, ORIGIN_X+hour_x1, ORIGIN_Y-hour_y1, HOUR_HAND_WIDTH, GxEPD_BLACK);
  //minute
  float minute_radians = (time.Minute/60.0)*2*M_PI; //radians
  int minute_x1 = sinf(minute_radians)*MINUTE_HAND_LENGTH;
  int minute_y1 = cosf(minute_radians)*MINUTE_HAND_LENGTH;
  fillRectAngle(ORIGIN_X, ORIGIN_Y, ORIGIN_X+minute_x1, ORIGIN_Y-minute_y1, MINUTE_HAND_WIDTH, GxEPD_BLACK);
}

void fillRectAngle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t size, uint16_t color)
{
      //x0,y0 to x1,y1 form the center line of the rectangle (width), size is the height
      int16_t dif_x = x1 - x0;                           // numerator of the formula for dy
      int16_t dif_y = y1 - y0;                           // numerator for dx
      float sqr = sqrt(dif_x*dif_x + dif_y*dif_y);   // denominator of the formulas
      int16_t dx = (size / 2.0) * dif_y / sqr;           // multiply the fraction with half the width of the rectangle
      int16_t dy = (size / 2.0) * dif_x / sqr;                 // the same for
      display.fillTriangle(x0 + dx, y0 - dy, x0 - dx, y0 + dy, x1 + dx, y1 - dy, color);
      display.fillTriangle(x0 - dx, y0 + dy, x1 - dx, y1 + dy, x1 + dx, y1 - dy, color);
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
