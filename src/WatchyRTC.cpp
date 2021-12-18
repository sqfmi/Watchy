#include "WatchyRTC.h"

WatchyRTC::WatchyRTC() {}

// TODO: We can probably put all of this logic into AbstractRTC as a class
// function. It would simplify this class even more, which would be nice
bool WatchyRTC::_canConnectTo(int addr) {
    byte error;
    Wire.beginTransmission(addr);
    error = Wire.endTransmission(addr);
    return error == 0;
}

void WatchyRTC::init(){
    if (_canConnectTo(RTC_DS_ADDR)) {
        rtcType = DS3232_RTC_TYPE;
        _rtc = new DS3232();
        return;
    }

    if (_canConnectTo(RTC_PCF_ADDR)) {
        rtcType = PCF8563_RTC_TYPE;
        _rtc = new PCF8563();
        return;
    }

    rtcType = NO_RTC_TYPE;
    _rtc = new AbstractRTC();
}

void WatchyRTC::config(String datetime){
    _rtc->config(datetime);
}

void WatchyRTC::clearAlarm(){
    _rtc->clearAlarm(); 
}

void WatchyRTC::read(tmElements_t &tm){
    if (rtcType == DS3232_RTC_TYPE) {
        ((DS3232*) _rtc)->rtc_ds.read(tm);
        tm.Year = tm.Year - 30; //reset to offset from 2000
    } else if (rtcType == PCF8563_RTC_TYPE) {
        tm.Month = ((PCF8563 *) _rtc)->rtc_pcf.getMonth();
        if(tm.Month == 0){ //PCF8563 POR sets month = 0 for some reason
            tm.Month = 1;
            tm.Year = 21;
        }else{
            tm.Year = ((PCF8563 *) _rtc)->rtc_pcf.getYear();
        }
        tm.Day = ((PCF8563 *) _rtc)->rtc_pcf.getDay();
        tm.Wday = ((PCF8563 *) _rtc)->rtc_pcf.getWeekday() + 1;
        tm.Hour = ((PCF8563 *) _rtc)->rtc_pcf.getHour();
        tm.Minute = ((PCF8563 *) _rtc)->rtc_pcf.getMinute();
        tm.Second = ((PCF8563 *) _rtc)->rtc_pcf.getSecond();
    }    
}

void WatchyRTC::set(tmElements_t tm){
    _rtc->set(tm);
}

uint8_t WatchyRTC::temperature(){
    if (rtcType == DS3232_RTC_TYPE) {
        return ((DS3232*) _rtc)->rtc_ds.temperature();
    }

    return NO_TEMPERATURE_ERR;
}