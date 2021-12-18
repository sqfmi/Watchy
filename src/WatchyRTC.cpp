#include "WatchyRTC.h"

WatchyRTC::WatchyRTC() {}

void WatchyRTC::init(){
    _rtc = AbstractRTC::init();
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

int WatchyRTC::rtcType() {
    return _rtc->rtcType();
}