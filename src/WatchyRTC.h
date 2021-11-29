#ifndef WATCHY_RTC_H
#define WATCHY_RTC_H

#include <DS3232RTC.h>
#include <Rtc_Pcf8563.h>

#define DS3231 0
#define PCF8563 1
#define RTC_DS_ADDR 0x68
#define RTC_PCF_ADDR 0x51
#define YEAR_OFFSET_DS 1970
#define YEAR_OFFSET_PCF 2000

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