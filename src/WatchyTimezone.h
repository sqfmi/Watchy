#pragma once
#include <stdlib.h>
#include <time.h>

// The ESP32 C library accepts POSIX TZ rules, not IANA zone names.
inline bool watchyLocalTime(time_t utc, const char *zone, struct tm &local) {
  if (!zone || !*zone || setenv("TZ", zone, 1) != 0) return false;
  tzset();
  return localtime_r(&utc, &local) != nullptr;
}

inline time_t watchyUtcTime(struct tm local, const char *zone) {
  setenv("TZ", zone, 1);
  tzset();
  local.tm_isdst = -1;
  return mktime(&local);
}
