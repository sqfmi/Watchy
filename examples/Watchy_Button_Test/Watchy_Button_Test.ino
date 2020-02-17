/*
 * Watchy - Button Test Example 
 * Watchy Basic + testing all 4 buttons
 *
 *                ==============
 *               |--------------|
 *  (IO34) BTN 3 |              | BTN 1 (IO39)
 *               |              |
 *               |              |
 *               |              |
 *  (IO35) BTN 4 |              | BTN 2 (IO36)
 *               |--------------|
 *                ==============  
 */

#ifndef ESP32
#error Please select ESP32 Wrover Module under Tools > Board
#endif
 
#include <DS3232RTC.h>
#include <GxEPD2_BW.h>
#include "DSEG7_Classic_Bold_48.h"
#include "DSEG14_Classic_Bold_18.h"

#define VIB_MOTOR_PIN 27
#define RTC_PIN GPIO_NUM_33
#define BTN_1 GPIO_SEL_39
#define BTN_2 GPIO_SEL_36
#define BTN_3 GPIO_SEL_34
#define BTN_4 GPIO_SEL_35
#define BTN_PIN_MASK BTN_1|BTN_2|BTN_3|BTN_4

DS3232RTC RTC(false);
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEH0154D67


void setup()
{
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0: updateTime(false); break; //RTC Alarm Interrupt
    case ESP_SLEEP_WAKEUP_EXT1: handleButton(); break; //Button Press
    default: updateTime(true); //Hard Reset
  }

  esp_sleep_enable_ext0_wakeup(RTC_PIN, 0); //enable deep sleep wake on RTC interrupt
  esp_sleep_enable_ext1_wakeup(BTN_PIN_MASK, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press
  esp_deep_sleep_start();
}

void loop(){}

void handleButton()
{
  uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();

  pinMode(VIB_MOTOR_PIN, OUTPUT);
  digitalWrite(VIB_MOTOR_PIN, HIGH);
  delay(100);
  digitalWrite(VIB_MOTOR_PIN, LOW);

  display.init();
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&DSEG14_Classic_Bold_18);
  
  display.setCursor(15, 100);
  display.println("You  pressed");
  display.setCursor(65, 140);
  if (wakeupBit & BTN_4) {
    display.println("BTN 4");
  } 
  else if (wakeupBit & BTN_3) {
    display.println("BTN 3");
  }
  else if (wakeupBit & BTN_2) {
    display.println("BTN 2");
  }
  else if (wakeupBit & BTN_1) {
    display.println("BTN 1");   
  }
  display.display(false);
  display.hibernate();
}

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

  display.init(0, reset); //_initial_refresh to false to prevent full update on init
  display.setFullWindow();
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