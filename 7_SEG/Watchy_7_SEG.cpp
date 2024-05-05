#include "Watchy_7_SEG.h"

#define DARKMODE true

const uint8_t BATTERY_SEGMENT_WIDTH = 7;
const uint8_t BATTERY_SEGMENT_HEIGHT = 11;
const uint8_t BATTERY_SEGMENT_SPACING = 9;
const uint8_t WEATHER_ICON_WIDTH = 48;
const uint8_t WEATHER_ICON_HEIGHT = 32;

void Watchy7SEG::drawWatchFace(){
    Serial.begin(115200);

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

    setupFS();
    syncAPI();
}

void Watchy7SEG::drawTime(){
    display.setFont(&DSEG7_Classic_Bold_53);
    display.setCursor(5, 53+5);
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
    if(currentTime.Wday == 4){
        w = w - 5;
    }
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
    display.println(tmYearToCalendar(currentTime.Year));// offset from 1970, since year is stored in uint8_t
}
void Watchy7SEG::drawSteps(){
    // reset step counter at midnight
    if (currentTime.Hour == 0 && currentTime.Minute == 0){
    	sensor.resetStepCounter();
    }
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
    display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
    if(159 - w - x1 > 87){
        display.setCursor(159 - w - x1, 150);
    }else{
        display.setFont(&DSEG7_Classic_Bold_25);
        display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
        display.setCursor(159 - w - x1, 136);
    }
    display.println(temperature);
    display.drawBitmap(165, 110, currentWeather.isMetric ? celsius : fahrenheit, 26, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    const unsigned char* weatherIcon;

    if(WIFI_CONFIGURED){
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
    }else{
    	weatherIcon = chip;
    }
    
    display.drawBitmap(145, 158, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}

void Watchy7SEG::setupFS(){
	LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED);
	FSData::createDir(LittleFS, DATA_FOLDER);
	FSData::createDir(LittleFS, STEPS_FOLDER);
}

void Watchy7SEG::syncAPI(){
	if (currentTime.Minute == 59 && sensor.getCounter() > 0){
		String day_api = (currentTime.Day < 10) ? ("0" + String(currentTime.Day)) : String(currentTime.Day);
		String month_api = (currentTime.Month < 10) ? ("0" + String(currentTime.Month)) : String(currentTime.Month);
		String date_api = day_api + "-" + month_api + "-" + String(currentTime.Year);

		String hour_api = (currentTime.Hour < 10) ? ("0" + String(currentTime.Hour)) : String(currentTime.Hour);
		String json_steps = "{\"event_type\":" + String(ENDPOINT_API) + ",\"client_payload\":{\"data-name\":" + String(ENDPOINT_STEPS) + ",\"date\":" + date_api + ",\"hour\":" + hour_api + ",\"data\":" + String(sensor.getCounter()) + "}}";

		//file_system = FSData();
		if (WIFI_CONFIGURED) {
			// Steps
			SendData::pushAPIData(json_steps);
			// Sync old steps
			//for(const std::string& file : file_system.listDir(STEPS_FOLDER), 1) {
			//	String json_file = file_system.readFile(String(STEPS_FOLDER) + "/" + file.c_str());
			//	SendData::pushAPIData(json_file);
			//	FSData::deleteFile(String(STEPS_FOLDER) + "/" + file.c_str());
			//}

			// turn off radios
			WiFi.mode(WIFI_OFF);
			btStop();
		} else { // No WiFi, register in file
			String file_name = (String(STEPS_FOLDER) + date_api + "_" + hour_api + ".txt").c_str();
			if (LittleFS.exists(file_name)){
			//	FSData::appendFile(LittleFS, file_name, json_steps.c_str());
			} else {
				//FSData::writeFile(LittleFS, file_name, json_steps.c_str());
			}
		}
	}
}
