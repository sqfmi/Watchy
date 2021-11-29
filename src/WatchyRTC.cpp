#include "WatchyRTC.h"

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
        int nextAlarmMinute = 0;
        rtc_pcf.clearAlarm(); //resets the alarm flag in the RTC
        nextAlarmMinute = rtc_pcf.getMinute();
        nextAlarmMinute = (nextAlarmMinute == 59) ? 0 : (nextAlarmMinute + 1); //set alarm to trigger 1 minute from now
        rtc_pcf.setAlarm(nextAlarmMinute, 99, 99, 99);
    }    
}

void WatchyRTC::read(tmElements_t &tm){
    if(rtcType == DS3231){
        rtc_ds.read(tm);
        tm.Year = tm.Year - 30; //reset to offset from 2000
    }else{
        tm.Month = rtc_pcf.getMonth();
        tm.Day = rtc_pcf.getDay();
        tm.Year = rtc_pcf.getYear();
        tm.Hour = rtc_pcf.getHour();
        tm.Minute = rtc_pcf.getMinute();
        tm.Second = rtc_pcf.getSecond();
        tm.Wday = rtc_pcf.getWeekday() + 1;
    }    
}

void WatchyRTC::set(tmElements_t tm){
    if(rtcType == DS3231){
        tm.Year = tm.Year + 2000 - YEAR_OFFSET_DS;
        time_t t = makeTime(tm);
        rtc_ds.set(t);
    }else{
        rtc_pcf.setDate(tm.Day, _getDayOfWeek(tm.Day, tm.Month, tm.Year+YEAR_OFFSET_PCF), tm.Month, 0, tm.Year);
        rtc_pcf.setTime(tm.Hour, tm.Minute, tm.Second);        
    }
}

uint8_t WatchyRTC::temperature(){
    if(rtcType == DS3231){
        return rtc_ds.temperature();
    }else{
        return 25;      
    }
}

void WatchyRTC::_DSConfig(String datetime){
    if(datetime != NULL){
        tmElements_t tm;
        tm.Year = _getValue(datetime, ':', 0).toInt() - YEAR_OFFSET_DS;//offset from 1970, since year is stored in uint8_t        
        tm.Month = _getValue(datetime, ':', 1).toInt();
        tm.Day = _getValue(datetime, ':', 2).toInt();
        tm.Hour = _getValue(datetime, ':', 3).toInt();
        tm.Minute = _getValue(datetime, ':', 4).toInt();
        tm.Second = _getValue(datetime, ':', 5).toInt();
        time_t t = makeTime(tm);
        rtc_ds.set(t);
    }
    //https://github.com/JChristensen/DS3232RTC
    rtc_ds.squareWave(SQWAVE_NONE); //disable square wave output
    rtc_ds.setAlarm(ALM2_EVERY_MINUTE, 0, 0, 0, 0); //alarm wakes up Watchy every minute
    rtc_ds.alarmInterrupt(ALARM_2, true); //enable alarm interrupt  
}

void WatchyRTC::_PCFConfig(String datetime){
    if(datetime != NULL){
        tmElements_t tm;
        tm.Year = _getValue(datetime, ':', 0).toInt();   
        tm.Month = _getValue(datetime, ':', 1).toInt();
        tm.Day = _getValue(datetime, ':', 2).toInt();
        tm.Hour = _getValue(datetime, ':', 3).toInt();
        tm.Minute = _getValue(datetime, ':', 4).toInt();
        tm.Second = _getValue(datetime, ':', 5).toInt();

        rtc_pcf.initClock();
        //day, weekday, month, century(1=1900, 0=2000), year(0-99)
        rtc_pcf.setDate(tm.Day, _getDayOfWeek(tm.Day, tm.Month, tm.Year), tm.Month, 0, tm.Year - YEAR_OFFSET_PCF);//offset from 2000, since year is stored in uint8_t
        //hr, min, sec
        rtc_pcf.setTime(tm.Hour, tm.Minute, tm.Second);

    }
    rtc_pcf.clearAlarm();
    int nextAlarmMinute = rtc_pcf.getMinute();
    nextAlarmMinute = (nextAlarmMinute == 59) ? 0 : (nextAlarmMinute + 1);
    rtc_pcf.setAlarm(nextAlarmMinute, 99, 99, 99);
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