#ifndef WATCHY_RTC_H
#define WATCHY_RTC_H

#include <DS3232RTC.h>
#include <Rtc_Pcf8563.h>

#define DS3232_RTC_TYPE 0
#define PCF8563_RTC_TYPE 1
#define NO_RTC_TYPE 255

#define RTC_DS_ADDR 0x68
#define RTC_PCF_ADDR 0x51
#define YEAR_OFFSET_DS3232 1970
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
protected:
    String getValue(String data, char separator, int index) {
        int found = 0;
        int strIndex[] = {0, -1};
        int maxIndex = data.length()-1;

        for (int i=0; i <= maxIndex && found <= index; i++) {
            if (data.charAt(i)==separator || i==maxIndex) {
                found++;
                strIndex[0] = strIndex[1] + 1;
                strIndex[1] = (i == maxIndex) ? i + 1 : i;
            }
        }

        if (found > index) {
            return data.substring(strIndex[0], strIndex[1]);
        }

        return "";
    }
};

class DS3232 : public AbstractRTC {
public:
    DS3232RTC rtc_ds; // TODO: We should not have public member variables
    ~DS3232() {}

    void config(String datetime) {
        if (datetime != "") {
            tmElements_t tm;
            // offset from 1970, since year is stored in uint8_t
            tm.Year = getValue(datetime, ':', 0).toInt() - YEAR_OFFSET_DS3232;
            tm.Month = getValue(datetime, ':', 1).toInt();
            tm.Day = getValue(datetime, ':', 2).toInt();
            tm.Hour = getValue(datetime, ':', 3).toInt();
            tm.Minute = getValue(datetime, ':', 4).toInt();
            tm.Second = getValue(datetime, ':', 5).toInt();
            time_t t = makeTime(tm);
            rtc_ds.set(t);
        }

        // https://github.com/JChristensen/DS3232RTC
        rtc_ds.squareWave(SQWAVE_NONE); //disable square wave output
        rtc_ds.setAlarm(ALM2_EVERY_MINUTE, 0, 0, 0, 0); //alarm wakes up Watchy every minute
        rtc_ds.alarmInterrupt(ALARM_2, true); //enable alarm interrupt
    }

    void clearAlarm() {
        rtc_ds.alarm(ALARM_2);
    }

    void read(tmElements_t &tm) {
        rtc_ds.read(tm);
        tm.Year = tm.Year - 30; //reset to offset from 2000
    }

    void set(tmElements_t tm) {
        tm.Year = tm.Year + 2000 - YEAR_OFFSET_DS3232;
        time_t t = makeTime(tm);
        rtc_ds.set(t);
    }

    uint8_t temperature() {
        return rtc_ds.temperature();
    }
};

class PCF8563 : public AbstractRTC {
public:
    Rtc_Pcf8563 rtc_pcf; // TODO: We should not have public member variables
    ~PCF8563() {}

    void config(String datetime) {
        if (datetime != "") {
            int Year = getValue(datetime, ':', 0).toInt();
            int Month = getValue(datetime, ':', 1).toInt();
            int Day = getValue(datetime, ':', 2).toInt();
            int Hour = getValue(datetime, ':', 3).toInt();
            int Minute = getValue(datetime, ':', 4).toInt();
            int Second = getValue(datetime, ':', 5).toInt();
            //day, weekday, month, century(1=1900, 0=2000), year(0-99)
            int dayOfWeek = getDayOfWeek(Day, Month, Year);

            // offset from 2000
            rtc_pcf.setDate(Day, dayOfWeek, Month, 0, Year - YEAR_OFFSET_PCF);
            //hr, min, sec
            rtc_pcf.setTime(Hour, Minute, Second);
        }

        clearAlarm();
    }

    void clearAlarm() {
        int nextAlarmMinute = 0;
        rtc_pcf.clearAlarm(); // resets the alarm flag in the RTC
        nextAlarmMinute = rtc_pcf.getMinute();
        nextAlarmMinute = (nextAlarmMinute == 59) ? 0 : (nextAlarmMinute + 1); //set alarm to trigger 1 minute from now
        rtc_pcf.setAlarm(nextAlarmMinute, 99, 99, 99);
    }

    void read(tmElements_t &tm) {
        tm.Month = rtc_pcf.getMonth();
        if (tm.Month == 0){ //PCF8563 POR sets month = 0 for some reason
            tm.Month = 1;
            tm.Year = 21; // TODO: I feel nervous about this--it's only 21 for a year, right?
        } else {
            tm.Year = rtc_pcf.getYear();
        }
        tm.Day = rtc_pcf.getDay();
        tm.Wday = rtc_pcf.getWeekday() + 1;
        tm.Hour = rtc_pcf.getHour();
        tm.Minute = rtc_pcf.getMinute();
        tm.Second = rtc_pcf.getSecond();
    }

    void set(tmElements_t tm) {
        int dayOfWeek = getDayOfWeek(tm.Day, tm.Month, tm.Year + YEAR_OFFSET_PCF);
        rtc_pcf.setDate(tm.Day, dayOfWeek, tm.Month, 0, tm.Year);
        rtc_pcf.setTime(tm.Hour, tm.Minute, tm.Second);
        clearAlarm();
    }
private:
    int getDayOfWeek(int d, int m, int y) {
        static int t[] = { 0, 3, 2, 5, 0, 3,
                            5, 1, 4, 6, 2, 4 };
        y -= m < 3;
        return ( y + y / 4 - y / 100 +
                 y / 400 + t[m - 1] + d) % 7;
    }
};

class WatchyRTC {
    public:
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
        bool _canConnectTo(int addr);
        AbstractRTC* _rtc;
};

#endif