#ifndef CONFIG_H
#define CONFIG_H

//pins
#define SDA 21
#define SCL 22
#define ADC_PIN 33
#define RTC_PIN GPIO_NUM_27
#define CS 5
#define DC 10
#define RESET 9
#define BUSY 19
#define VIB_MOTOR_PIN 13
#define MENU_BTN_PIN 26
#define BACK_BTN_PIN 25
#define UP_BTN_PIN 32
#define DOWN_BTN_PIN 4
#define MENU_BTN_MASK GPIO_SEL_26
#define BACK_BTN_MASK GPIO_SEL_25
#define UP_BTN_MASK GPIO_SEL_32
#define DOWN_BTN_MASK GPIO_SEL_4
#define ACC_INT_MASK GPIO_SEL_14
#define BTN_PIN_MASK MENU_BTN_MASK|BACK_BTN_MASK|UP_BTN_MASK|DOWN_BTN_MASK
//display
#define DISPLAY_WIDTH 200
#define DISPLAY_HEIGHT 200
//weather api
#define CITY_NAME "NEW+YORK" //if your city name has a space, replace with '+'
#define COUNTRY_CODE "US"
#define OPENWEATHERMAP_APIKEY "f058fe1cad2afe8e2ddc5d063a64cecb" //use your own API key :)
#define OPENWEATHERMAP_URL "http://api.openweathermap.org/data/2.5/weather?q="
#define TEMP_UNIT "metric" //use "imperial" for Fahrenheit"
#define WEATHER_UPDATE_INTERVAL 30 //minutes
//wifi
#define WIFI_AP_TIMEOUT 60
#define WIFI_AP_SSID "Watchy AP"
//menu
#define WATCHFACE_STATE -1
#define MAIN_MENU_STATE 0
#define APP_STATE 1
#define FW_UPDATE_STATE 2
#define MENU_HEIGHT 30
#define MENU_LENGTH 6
//set time
#define SET_HOUR 0
#define SET_MINUTE 1
#define SET_YEAR 2
#define SET_MONTH 3
#define SET_DAY 4
#define YEAR_OFFSET 1970
//BLE OTA
#define BLE_DEVICE_NAME "Watchy BLE OTA"
#define WATCHFACE_NAME "Watchy 7 Segment"
#define SOFTWARE_VERSION_MAJOR 1
#define SOFTWARE_VERSION_MINOR 0
#define SOFTWARE_VERSION_PATCH 0
#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 0

#endif