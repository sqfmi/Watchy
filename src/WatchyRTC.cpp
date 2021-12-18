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
    _rtc->read(tm);
}

void WatchyRTC::set(tmElements_t tm){
    _rtc->set(tm);
}

uint8_t WatchyRTC::temperature(){
    return _rtc->temperature();
}