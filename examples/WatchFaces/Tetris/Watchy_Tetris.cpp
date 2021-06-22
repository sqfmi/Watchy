#include "Watchy_Tetris.h"

const unsigned char *tetris_nums [10] = {tetris0, tetris1, tetris2, tetris3, tetris4, tetris5, tetris6, tetris7, tetris8, tetris9};

WatchyTetris::WatchyTetris(){} //constructor

void WatchyTetris::drawWatchFace(){
    display.fillScreen(GxEPD_WHITE);
    display.drawBitmap(0, 0, tetrisbg, DISPLAY_WIDTH, DISPLAY_HEIGHT, GxEPD_BLACK);

    //Hour
    display.drawBitmap(25, 20, tetris_nums[currentTime.Hour/10], 40, 60, GxEPD_BLACK); //first digit
    display.drawBitmap(75, 20, tetris_nums[currentTime.Hour%10], 40, 60, GxEPD_BLACK); //second digit

    //Minute
    display.drawBitmap(25, 110, tetris_nums[currentTime.Minute/10], 40, 60, GxEPD_BLACK); //first digit
    display.drawBitmap(75, 110, tetris_nums[currentTime.Minute%10], 40, 60, GxEPD_BLACK); //second digit

    syncNtpTime();
}

void Watchy7SEG::syncNtpTime(){
    const char* ntpServer = "pool.ntp.org";
    const long gmtOffset_sec = -21600; // set time zone to central standard time
                                        // ie UTC -6 * 60 * 60 = -21600
    const int daylightOffset_sec = 3600;// if observing Daylight saving time 3600 otherwise 0
    struct tm timeinfo;

    // Watchy updates every minute but we really only need to sync a few times a day
    if(currentTime.Hour % 8 == 0 && currentTime.Minute == 0){
      //only run this at midnight,8am and 4pm
      // three times a day to correct time drift.

      //make sure WiFi is connected
      if(Watchy::connectWiFi()){
          // wifi connected so proceed to get NTP time
          
          //get NTP Time
          configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
          getLocalTime(&timeinfo);

          // convert NTP time into proper format
          tmElements_t tm;
          tm.Month = timeinfo.tm_mon + 1;// 0-11 based month so we have to add 1
          tm.Day = timeinfo.tm_mday;
          tm.Year = timeinfo.tm_year + 1900 - YEAR_OFFSET;//offset from 1970, since year is stored in uint8_t
          tm.Hour = timeinfo.tm_hour;
          tm.Minute = timeinfo.tm_min;
          tm.Second = timeinfo.tm_sec;
          time_t t = makeTime(tm);
          
          //set the RTC time to the NTP time
          RTC.set(t);

          // shut down the radio to save power
          WiFi.mode(WIFI_OFF);
          btStop();
      }
    }
}