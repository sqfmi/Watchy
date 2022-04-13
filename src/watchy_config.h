#pragma once

//wifi
#define WIFI_AP_TIMEOUT 60
#define WIFI_AP_SSID "Watchy AP"
//menu
#define MAIN_MENU_STATE 0
#define APP_STATE 1
#define FW_UPDATE_STATE 2
#define MENU_HEIGHT 25
#define MENU_LENGTH 7
//set time
#define SET_HOUR 0
#define SET_MINUTE 1
#define SET_YEAR 2
#define SET_MONTH 3
#define SET_DAY 4
#define HOUR_12_24 24
//BLE OTA
#define BLE_DEVICE_NAME "Watchy BLE OTA"
#define WATCHFACE_NAME "Watchy 7 Segment"
#define SOFTWARE_VERSION_MAJOR 1
#define SOFTWARE_VERSION_MINOR 0
#define SOFTWARE_VERSION_PATCH 0
#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 0
//Versioning
#define WATCHY_LIB_VER "1.4.0"

namespace watchy_config
{

namespace display
{
// Display
#define DISPLAY_WIDTH 200
#define DISPLAY_HEIGHT 200

// display pins
constexpr std::uint8_t cs{5}; // Unsure what this means.
constexpr std::uint8_t dc{10}; // Unsure what this means.
constexpr std::uint8_t reset{9};
constexpr std::uint8_t busy{19};
};

namespace pins
{

// RTC pins
constexpr gpio_num_t rtc_pin{GPIO_NUM_27};

#define ARDUINO_WATCHY_V20 1

#if defined (ARDUINO_WATCHY_V10)
	constexpr std::uint64_t up_btn_mask{GPIO_SEL_32};
#elif defined (ARDUINO_WATCHY_V15)
	constexpr std::uint64_t up_btn_mask{GPIO_SEL_32};
#elif defined (ARDUINO_WATCHY_V20)
	constexpr std::uint64_t up_btn_mask{GPIO_SEL_35};
	constexpr std:: uint8_t batt_adc_pin{34};
#endif

// btn pins & masks
constexpr std::uint64_t menu_btn_mask{GPIO_SEL_26};
constexpr std::uint64_t back_btn_mask{GPIO_SEL_25};
constexpr std::uint64_t down_btn_mask{GPIO_SEL_4};
constexpr std::uint64_t btn_pin_mask{menu_btn_mask|back_btn_mask|up_btn_mask|down_btn_mask};

};
};

namespace wc  = watchy_config;
namespace wcd = watchy_config::display;
namespace wcp = watchy_config::pins;
