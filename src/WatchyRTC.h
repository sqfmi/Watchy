#ifndef WATCHY_RTC_H
#define WATCHY_RTC_H

#include <DS3232RTC.h>
#include <Rtc_Pcf8563.h>

#define DS3231 0
#define PCF8563 1
#define RTC_DS_ADDR 0x68
#define RTC_PCF_ADDR 0x51
#define RTC_YEAR_OFFSET 1900			// Use for calculation to display year properly.
#define RTC_CENTURY_OFFSET 100			// Short Year offset.
#define RTC_1900TO1970 2208988800UL		// Subtract from NTP to get Epoch.
#define RTC_1970TO2000 946684800UL		// Subtract from Epoch to get Y2k.
#define RTC_1900TO2000 (RTC_1900TO1970 + RTC_1970TO2000)	// Subtract from NTP to get Y2k.

class WatchyRTC {
    public:
        DS3232RTC rtc_ds;
        Rtc_Pcf8563 rtc_pcf;
        uint8_t rtcType;
    public:
        WatchyRTC();
        void init();
        void config(String datetime);
        void clearAlarm();
        void read(tmElements_t &tm);
        void set(tmElements_t tm);
        uint8_t temperature();
    private:
        void _DSConfig(String datetime);
        void _PCFConfig(String datetime);
        int _getDayOfWeek(int d, int m, int y);
        String _getValue(String data, char separator, int index);
};

#endif
