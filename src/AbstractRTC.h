#ifndef ABSTRACT_RTC_H
#define ABSTRACT_RTC_H
#define YEAR_OFFSET_DS3232 1970
#define YEAR_OFFSET_PCF 2000

#define NO_TEMPERATURE_ERR 255

#define RTC_TYPE_DS3232 0
#define RTC_TYPE_PCF8563 1
#define RTC_TYPE_NONE -1

#include <Arduino.h>
#include <TimeLib.h>

class AbstractRTC {
public:
    virtual ~AbstractRTC() {}
    virtual void config(String datetime) {}
    virtual void clearAlarm() {}
    virtual void read(tmElements_t &tm) {}
    virtual void set(tmElements_t tm) {}
    virtual uint8_t temperature() { return NO_TEMPERATURE_ERR; }
    virtual int rtcType(){ return RTC_TYPE_NONE; }
    static AbstractRTC* init();
protected:
    String getValue(String data, char separator, int index);
};

#endif