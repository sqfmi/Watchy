#ifndef WATCHY_RTC_H
#define WATCHY_RTC_H

#include <Arduino.h>
#include <TimeLib.h>
#include "AbstractRTC.h"

class WatchyRTC {
    public:
        WatchyRTC();
        void init();
        void config(String datetime);
        void clearAlarm();
        void read(tmElements_t &tm);
        void set(tmElements_t tm);
        uint8_t temperature();
        int rtcType();
    private:
        AbstractRTC* _rtc;
};

#endif