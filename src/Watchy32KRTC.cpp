#include "Watchy32KRTC.h"

Watchy32KRTC::Watchy32KRTC(){}

void Watchy32KRTC::init() {

}

/*

  setenv("TZ", "", 1);
  tzset();

*/

void Watchy32KRTC::config(String datetime) { // String datetime format is YYYY:MM:DD:HH:MM:SS
    struct tm timeInfo;
    memset(&timeInfo, 0, sizeof(timeInfo));

    // Parse the time string
    if (strptime(datetime.c_str(), "%Y:%m:%d:%H:%M:%S", &timeInfo) == NULL) {
        // Failed to parse the time string
    }

    // Convert tm to timeval
    struct timeval tv;
    tv.tv_sec = mktime(&timeInfo);
    tv.tv_usec = 0;

    // Set the time using settimeofday
    if (settimeofday(&tv, NULL) != 0) {
        // Error setting the time
    }
}

void Watchy32KRTC::clearAlarm() {

}

void Watchy32KRTC::read(tmElements_t &tm) {
  time_t now;
  struct tm timeInfo;
  time(&now);
  // Set timezone to China Standard Time
  //setenv("TZ", "CST-8", 1);
  //tzset();
  localtime_r(&now, &timeInfo);
  tm.Year   = timeInfo.tm_year - 70;
  tm.Month  = timeInfo.tm_mon + 1;
  tm.Day    = timeInfo.tm_mday;
  tm.Wday   = timeInfo.tm_wday + 1;
  tm.Hour   = timeInfo.tm_hour;
  tm.Minute = timeInfo.tm_min;
  tm.Second = timeInfo.tm_sec;
}

void Watchy32KRTC::set(tmElements_t tm) {
  struct tm timeInfo;
  timeInfo.tm_year = tm.Year + 70;
  timeInfo.tm_mon  = tm.Month - 1;
  timeInfo.tm_mday = tm.Day;
  timeInfo.tm_hour = tm.Hour;
  timeInfo.tm_min  = tm.Minute;
  timeInfo.tm_sec  = tm.Second;

  // Convert tm to timeval
  struct timeval tv;
  tv.tv_sec = mktime(&timeInfo);
  tv.tv_usec = 0;
  
  // Set the time using settimeofday
  if (settimeofday(&tv, NULL) != 0) {
      // Error setting the time
  }  
}

uint8_t Watchy32KRTC::temperature() {
 return 0;
}

String Watchy32KRTC::_getValue(String data, char separator, int index) {
  int found      = 0;
  int strIndex[] = {0, -1};
  int maxIndex   = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void Watchy32KRTC::_timeval_to_tm(struct timeval *tv, struct tm *tm) {
  // Get the seconds and microseconds from the timeval struct
  time_t seconds = tv->tv_sec;
  int microseconds = tv->tv_usec;
  // Convert the seconds to a tm struct
  *tm = *localtime(&seconds);
}