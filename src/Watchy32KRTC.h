#ifndef WATCHY_32K_RTC_H
#define WATCHY_32K_RTC_H

#include <Arduino.h>
#include <TimeLib.h>
#include "config.h"

class Watchy32KRTC {
public:
  Watchy32KRTC();
  void init();
  void config(String datetime); //datetime format is YYYY:MM:DD:HH:MM:SS
  void clearAlarm();
  void read(tmElements_t &tm);
  void set(tmElements_t tm);
  uint8_t temperature();

private:
  String _getValue(String data, char separator, int index);
  void _timeval_to_tm(struct timeval *tv, struct tm *tm);
};

#endif