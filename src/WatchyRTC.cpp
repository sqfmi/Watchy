#include "WatchyRTC.h"

RTC_DATA_ATTR bool UsedCentury = false;

WatchyRTC::WatchyRTC()
    : rtc_ds(false) {}

void WatchyRTC::init(){
    byte error;
    Wire.beginTransmission(RTC_DS_ADDR);
    error = Wire.endTransmission();
    if(error == 0){
        rtcType = DS3231;
    }else{
        Wire.beginTransmission(RTC_PCF_ADDR);
        error = Wire.endTransmission();
        if(error == 0){
            rtcType = PCF8563;
        }else{
            //RTC Error
        }
    }
}

void WatchyRTC::config(String datetime){
    if(rtcType == DS3231){
        _DSConfig(datetime);
    }else{
        _PCFConfig(datetime);
    }
}

void WatchyRTC::clearAlarm(){
    if(rtcType == DS3231){
        rtc_ds.alarm(ALARM_2);
    }else{
        int nextAlarmMinute = constrain(rtc_pcf.getMinute() + 1, 0, 59); //set alarm to trigger 1 minute from now
        rtc_pcf.clearAlarm(); //resets the alarm flag in the RTC
        rtc_pcf.setAlarm(nextAlarmMinute, 99, 99, 99);
    }    
}

void WatchyRTC::read(tmElements_t &tm){	// Returns year from 1900 and beyond.
    if(rtcType == DS3231){
        rtc_ds.read(tm);
    }else{
        tm.Month = rtc_pcf.getMonth();
        if(tm.Month == 0){ //PCF8563 POR sets month = 0 for some reason
            tm.Month = 1;
            tm.Year = 121;	// From Epoch.
        }else{
            tm.Year = rtc_pcf.getYear() + (UsedCentury ? RTC_CENTURY_OFFSET : 0);	// Put 1900 year back on if used prior.
        }
        tm.Day = rtc_pcf.getDay();
        tm.Wday = rtc_pcf.getWeekday();
        tm.Hour = rtc_pcf.getHour();
        tm.Minute = rtc_pcf.getMinute();
        tm.Second = rtc_pcf.getSecond();
    }    
}

void WatchyRTC::set(tmElements_t tm){	// Requires dates from year 1900 and beyond.
    if(rtcType == DS3231){
        rtc_ds.write(tm);
    }else{
		UsedCentury = false;
		if (tm.Year > 99) {UsedCentury = true; tm.Year -= RTC_CENTURY_OFFSET; }
        rtc_pcf.setDate(tm.Day, _getDayOfWeek(tm.Day, tm.Month, tm.Year), tm.Month, (UsedCentury ? 0 : 1), tm.Year);
        rtc_pcf.setTime(tm.Hour, tm.Minute, tm.Second);
        clearAlarm();
    }
}

uint8_t WatchyRTC::temperature(){
    if(rtcType == DS3231){
        return rtc_ds.temperature();
    }else{
        return 255; //error
    }
}

void WatchyRTC::_DSConfig(String datetime){	// Requires dates from year 1900 and beyond.
    if(datetime != ""){
        tmElements_t tm;
        int Year = _getValue(datetime, ':', 0).toInt();
		if (Year >= RTC_YEAR_OFFSET) Year -= RTC_YEAR_OFFSET;
        tm.Month = _getValue(datetime, ':', 1).toInt();
        tm.Day = _getValue(datetime, ':', 2).toInt();
        tm.Hour = _getValue(datetime, ':', 3).toInt();
        tm.Minute = _getValue(datetime, ':', 4).toInt();
        tm.Second = _getValue(datetime, ':', 5).toInt();
		tm.Year = Year;
        time_t t = makeTime(tm);
        rtc_ds.set(t);
    }
    //https://github.com/JChristensen/DS3232RTC
    rtc_ds.squareWave(SQWAVE_NONE); //disable square wave output
    rtc_ds.setAlarm(ALM2_EVERY_MINUTE, 0, 0, 0, 0); //alarm wakes up Watchy every minute
    rtc_ds.alarmInterrupt(ALARM_2, true); //enable alarm interrupt  
}

void WatchyRTC::_PCFConfig(String datetime){	// Requires dates from year 1900 and beyond.
    if(datetime != ""){
        tmElements_t tm;
		UsedCentury = false;
        int Year = _getValue(datetime, ':', 0).toInt();
		int Y = Year;
		if (Year >= RTC_YEAR_OFFSET) Year -= RTC_YEAR_OFFSET;
		if (Year > 99) {UsedCentury = true; Year -= RTC_CENTURY_OFFSET; }
        int Month = _getValue(datetime, ':', 1).toInt();
        int Day = _getValue(datetime, ':', 2).toInt();
        int Hour = _getValue(datetime, ':', 3).toInt();
        int Minute = _getValue(datetime, ':', 4).toInt();
        int Second = _getValue(datetime, ':', 5).toInt();
        //day, weekday, month, century(1=1900, 0=2000), year(0-99)
        rtc_pcf.setDate(Day, _getDayOfWeek(Day, Month, Y), Month, (UsedCentury ? 0 : 1), Year);
        //hr, min, sec
        rtc_pcf.setTime(Hour, Minute, Second);
    }
    clearAlarm();
}

int WatchyRTC::_getDayOfWeek(int d, int m, int y)
{
    static int t[] = { 0, 3, 2, 5, 0, 3,
                       5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    return ( y + y / 4 - y / 100 +
             y / 400 + t[m - 1] + d) % 7;
}

String WatchyRTC::_getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
