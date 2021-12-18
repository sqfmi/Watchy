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
            rtc_pcf.setDate(Day, getDayOfWeek(Day, Month, Year), Month, 0, Year - YEAR_OFFSET_PCF);//offset from 2000
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
        int _getDayOfWeek(int d, int m, int y);
        AbstractRTC* _rtc;
};

#endif