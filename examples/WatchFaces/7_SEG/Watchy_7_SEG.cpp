#include "Watchy_7_SEG.h"

#define DARKMODE true

const uint8_t BATTERY_SEGMENT_WIDTH = 7;
const uint8_t BATTERY_SEGMENT_HEIGHT = 11;
const uint8_t BATTERY_SEGMENT_SPACING = 9;
const uint8_t WEATHER_ICON_WIDTH = 48;
const uint8_t WEATHER_ICON_HEIGHT = 32;

Watchy7SEG::Watchy7SEG(){} //constructor

void Watchy7SEG::drawWatchFace(){
    display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    drawTime();
    drawDate();
    drawSteps();
    drawWeather();
    drawBattery();
    display.drawBitmap(120, 77, WIFI_CONFIGURED ? wifi : wifioff, 26, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    if(BLE_CONFIGURED){
        display.drawBitmap(100, 75, bluetooth, 13, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
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

void Watchy7SEG::drawTime(){
    display.setFont(&DSEG7_Classic_Bold_53);
    display.setCursor(5, 53+5);
    if(currentTime.Hour < 10){
        display.print("0");
    }
    display.print(currentTime.Hour);
    display.print(":");
    if(currentTime.Minute < 10){
        display.print("0");
    }  
    display.println(currentTime.Minute);  
}

void Watchy7SEG::drawDate(){
    display.setFont(&Seven_Segment10pt7b);

    int16_t  x1, y1;
    uint16_t w, h;

    String dayOfWeek = dayStr(currentTime.Wday);
    display.getTextBounds(dayOfWeek, 5, 85, &x1, &y1, &w, &h);
    display.setCursor(85 - w, 85);
    display.println(dayOfWeek);

    String month = monthShortStr(currentTime.Month);
    display.getTextBounds(month, 60, 110, &x1, &y1, &w, &h);
    display.setCursor(85 - w, 110);
    display.println(month);

    display.setFont(&DSEG7_Classic_Bold_25);
    display.setCursor(5, 120);
    if(currentTime.Day < 10){
    display.print("0");      
    }     
    display.println(currentTime.Day);
    display.setCursor(5, 150);
    display.println(currentTime.Year + YEAR_OFFSET);// offset from 1970, since year is stored in uint8_t
}
void Watchy7SEG::drawSteps(){
    uint32_t stepCount = sensor.getCounter();
    display.drawBitmap(10, 165, steps, 19, 23, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(35, 190);
    display.println(stepCount);
}
void Watchy7SEG::drawBattery(){
    display.drawBitmap(154, 73, battery, 37, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.fillRect(159, 78, 27, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);//clear battery segments
    int8_t batteryLevel = 0;
    float VBAT = getBatteryVoltage();
    if(VBAT > 4.1){
        batteryLevel = 3;
    }
    else if(VBAT > 3.95 && VBAT <= 4.1){
        batteryLevel = 2;
    }
    else if(VBAT > 3.80 && VBAT <= 3.95){
        batteryLevel = 1;
    }    
    else if(VBAT <= 3.80){
        batteryLevel = 0;
    }

    for(int8_t batterySegments = 0; batterySegments < batteryLevel; batterySegments++){
        display.fillRect(159 + (batterySegments * BATTERY_SEGMENT_SPACING), 78, BATTERY_SEGMENT_WIDTH, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
}

void Watchy7SEG::drawWeather(){

    weatherData currentWeather = getWeatherData();

    int8_t temperature = currentWeather.temperature;
    int16_t weatherConditionCode = currentWeather.weatherConditionCode;   

    display.setFont(&DSEG7_Classic_Regular_39);
    int16_t  x1, y1;
    uint16_t w, h;
    display.getTextBounds(String(temperature), 100, 150, &x1, &y1, &w, &h);
    display.setCursor(155 - w, 150);
    display.println(temperature);
    display.drawBitmap(165, 110, strcmp(TEMP_UNIT, "metric") == 0 ? celsius : fahrenheit, 26, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    const unsigned char* weatherIcon;

    //https://openweathermap.org/weather-conditions
    if(weatherConditionCode > 801){//Cloudy
    weatherIcon = cloudy;
    }else if(weatherConditionCode == 801){//Few Clouds
    weatherIcon = cloudsun;  
    }else if(weatherConditionCode == 800){//Clear
    weatherIcon = sunny;  
    }else if(weatherConditionCode >=700){//Atmosphere
    weatherIcon = cloudy; 
    }else if(weatherConditionCode >=600){//Snow
    weatherIcon = snow;
    }else if(weatherConditionCode >=500){//Rain
    weatherIcon = rain;  
    }else if(weatherConditionCode >=300){//Drizzle
    weatherIcon = rain;
    }else if(weatherConditionCode >=200){//Thunderstorm
    weatherIcon = rain; 
    }else
    return;
    display.drawBitmap(145, 158, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}