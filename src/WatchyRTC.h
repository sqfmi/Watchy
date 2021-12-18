#ifndef WATCHY_RTC_H
#define WATCHY_RTC_H

#include <DS3232RTC.h>
#include <Rtc_Pcf8563.h>

#define DS3232_RTC_TYPE 0
#define PCF8563_RTC_TYPE 1
#define NO_RTC_TYPE 255

#define RTC_DS_ADDR 0x68
#define RTC_PCF_ADDR 0x51
#define YEAR_OFFSET_DS 1970
#define YEAR_OFFSET_PCF 2000

#define NO_TEMPERATURE_ERR 255

// TODO: So we're relying on an rtcType as a multiplexer, making our WatchyRTC code a bit
// more complex. A way around this is to use a command pattern instead:
// https://sourcemaking.com/design_patterns/command
//
// That way we don't have to have a bunch of conditionals floating around in this class.
// It will simplify the process of adding a new RTC chip later as well.

class AbstractRTC {
public:
    virtual ~AbstractRTC() {}
    virtual void config(String datetime) {}
    virtual void clearAlarm() {}
    virtual void read(tmElements_t &tm) {}
    virtual void set(tmElements_t tm) {}
    virtual uint8_t temperature() { return NO_TEMPERATURE_ERR; }
};

class DS3232 : public AbstractRTC {
public:
    DS3232RTC rtc_ds; // TODO: We should not have public member variables
    ~DS3232() {}
};

class PCF8563 : public AbstractRTC {
public:
    Rtc_Pcf8563 rtc_pcf; // TODO: We should not have public member variables
    ~PCF8563() {}
};

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
        bool _canConnectTo(int addr);
        AbstractRTC* _rtc;
};

#endif