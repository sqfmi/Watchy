#include "PCF8563.h"

void PCF8563::config(String datetime) {
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

void PCF8563::clearAlarm() {
	int nextAlarmMinute = 0;
  rtc_pcf.clearAlarm(); // resets the alarm flag in the RTC
  nextAlarmMinute = rtc_pcf.getMinute();
  nextAlarmMinute = (nextAlarmMinute == 59) ? 0 : (nextAlarmMinute + 1); //set alarm to trigger 1 minute from now
  rtc_pcf.setAlarm(nextAlarmMinute, 99, 99, 99);
}

void PCF8563::read(tmElements_t &tm) {
	tm.Month = rtc_pcf.getMonth();
  if (tm.Month == 0){ //PCF8563 POR sets month = 0 for some reason
    tm.Month = 1;
    tm.Year = 21;
  } else {
    tm.Year = rtc_pcf.getYear();
  }
  tm.Day = rtc_pcf.getDay();
  tm.Wday = rtc_pcf.getWeekday() + 1;
  tm.Hour = rtc_pcf.getHour();
  tm.Minute = rtc_pcf.getMinute();
  tm.Second = rtc_pcf.getSecond();
}

void PCF8563::set(tmElements_t tm) {
	int dayOfWeek = getDayOfWeek(tm.Day, tm.Month, tm.Year + YEAR_OFFSET_PCF);
  rtc_pcf.setDate(tm.Day, dayOfWeek, tm.Month, 0, tm.Year);
  rtc_pcf.setTime(tm.Hour, tm.Minute, tm.Second);
  clearAlarm();
}

int PCF8563::getDayOfWeek(int d, int m, int y) {
	static int t[] = { 0, 3, 2, 5, 0, 3,
                      5, 1, 4, 6, 2, 4 };
  y -= m < 3;
  return ( y + y / 4 - y / 100 +
           y / 400 + t[m - 1] + d) % 7;
}

int PCF8563::rtcType() {
	return RTC_TYPE_PCF8563;
}