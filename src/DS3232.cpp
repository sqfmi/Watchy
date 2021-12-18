#include "DS3232.h"

void DS3232::config(String datetime) {
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

void DS3232::clearAlarm() {
	rtc_ds.alarm(ALARM_2);
}

void DS3232::read(tmElements_t &tm) {
	rtc_ds.read(tm);
  tm.Year = tm.Year - 30; //reset to offset from 2000	
}

void DS3232::set(tmElements_t tm) {
	tm.Year = tm.Year + 2000 - YEAR_OFFSET_DS3232;
  time_t t = makeTime(tm);
  rtc_ds.set(t);
}

uint8_t DS3232::temperature() {
	return rtc_ds.temperature();
}

int DS3232::rtcType() {
	return RTC_TYPE_DS3232;
}