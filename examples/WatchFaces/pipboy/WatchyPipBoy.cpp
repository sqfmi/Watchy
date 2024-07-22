#include "WatchyPipBoy.h"

#define DARKMODE true
#define STEPSGOAL 5000

const uint8_t WEATHER_ICON_WIDTH = 48;
const uint8_t WEATHER_ICON_HEIGHT = 32;

RTC_DATA_ATTR uint8_t vaultBoyNum;

void WatchyPipBoy::drawWatchFace(){
    //top menu bar
    display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    display.setFont(&monofonto8pt7b);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(22, 14);
    display.print("STAT  INV  DATA  MAP");
    display.drawBitmap(0, 10, menubar, 200, 9, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

    //bottom text
    display.setFont(&monofonto8pt7b);
    display.setCursor(10, 195);
    display.println("PIP-BOY 3000 ROBCO IND.");

    drawTime();
    drawDate();
    drawSteps();
    drawWeather();
    drawBattery();
    // display.drawBitmap(120, 77, WIFI_CONFIGURED ? wifi : wifioff, 26, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    // if(BLE_CONFIGURED){
    //     display.drawBitmap(100, 75, bluetooth, 13, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    // }
}

void WatchyPipBoy::drawTime(){

    //draw random vault boy every 15mins
    if(currentTime.Minute % 15 == 0){
        vaultBoyNum = random(0, 3);
    }

    switch(vaultBoyNum){
        case 0:
            display.drawBitmap(70, 50, vaultboy, 57, 100, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
            break;
        case 1:
            display.drawBitmap(70, 50, vaultboypoint, 57, 100, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
            break;
        case 2:
            display.drawBitmap(60, 50, vaultboysmile, 67, 100, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
            break;
    }    

    //time border
    display.drawLine(137, 28, 200, 28, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.drawLine(137, 28, 137, 132, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.drawLine(137, 132, 157, 132, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);    
    display.drawLine(180, 132, 200, 132, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

    //draw time
    display.setFont(&monofonto28pt7b);
    display.setCursor(141, 75);

    int displayHour;
    if(HOUR_12_24==12){
      displayHour = ((currentTime.Hour+11)%12)+1;
    } else {
      displayHour = currentTime.Hour;
    }
    if(displayHour < 10){
        display.print("0");
    }
    display.print(displayHour);

    display.setCursor(141, 125);
    if(currentTime.Minute < 10){
        display.print("0");
    }
    display.print(currentTime.Minute);

    //AMPM
    display.setFont(&monofonto8pt7b);
    display.setCursor(160, 140);
    display.print(currentTime.Hour < 11 ? "AM" : "PM");
}

void WatchyPipBoy::drawDate(){

    display.setFont(&monofonto10pt7b);
    int16_t  x1, y1;
    uint16_t w, h;
    String dayOfWeek = dayStr(currentTime.Wday);
    dayOfWeek.toUpperCase();
    display.setTextColor(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    display.getTextBounds(dayOfWeek, 7, 42, &x1, &y1, &w, &h);
    display.setCursor(7, 42);
    display.fillRect(x1-2, y1-2, w+4, h+4, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.print(dayOfWeek);

    display.setFont(&monofonto10pt7b);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(7, 62);
    display.print(monthShortStr(currentTime.Month));
    display.print(" ");
    display.print(currentTime.Day);
    display.setCursor(7, 78);
    display.print(tmYearToCalendar(currentTime.Year));
}

void WatchyPipBoy::drawSteps(){
    // reset step counter at midnight
    if (currentTime.Hour == 0 && currentTime.Minute == 0){
      sensor.resetStepCounter();
    }

    //draw progress bar
    uint32_t stepCount = sensor.getCounter();
    uint8_t progress = (uint8_t)(stepCount * 100.0 / STEPSGOAL);
    progress = progress > 100 ? 100 : progress;
    display.drawBitmap(60, 155, gauge, 73, 10, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.fillRect(60+13, 155+5, (progress/2)+5, 4, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

    //show step count
    display.setFont(&monofonto8pt7b);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(150, 160);
    display.print("STEPS");
    display.setCursor(150, 175);
    display.print(stepCount);
}

void WatchyPipBoy::drawBattery(){
    display.drawBitmap(10, 150, battery, 37, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.fillRect(15, 155, 27, 11, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);//clear battery segments
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
        display.fillRect(15 + (batterySegments * 9), 155, 7, 11, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
}

void WatchyPipBoy::drawWeather(){

    weatherData currentWeather = getWeatherData();

    int8_t temperature = currentWeather.temperature;
    int16_t weatherConditionCode = currentWeather.weatherConditionCode;

    display.setFont(&monofonto10pt7b);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(12, 133);
 
 
    display.print(temperature);
    display.print(currentWeather.isMetric ? "C" : "F");
    const unsigned char* weatherIcon;

    //https://openweathermap.org/weather-conditions
    if(weatherConditionCode > 801){//Cloudy
    weatherIcon = cloudy;
    }else if(weatherConditionCode == 801){//Few Clouds
    weatherIcon = cloudsun;
    }else if(weatherConditionCode == 800){//Clear
    weatherIcon = sunny;
    }else if(weatherConditionCode >=700){//Atmosphere
    weatherIcon = atmosphere;
    }else if(weatherConditionCode >=600){//Snow
    weatherIcon = snow;
    }else if(weatherConditionCode >=500){//Rain
    weatherIcon = rain;
    }else if(weatherConditionCode >=300){//Drizzle
    weatherIcon = drizzle;
    }else if(weatherConditionCode >=200){//Thunderstorm
    weatherIcon = thunderstorm;
    }else
    return;
    display.drawBitmap(5, 85, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}
