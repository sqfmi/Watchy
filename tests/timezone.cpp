#include "../src/WatchyTimezone.h"
#include <assert.h>
#include <stdio.h>
#include <initializer_list>
static const char *ROME = "CET-1CEST,M3.5.0,M10.5.0/3";
static time_t utc(int month, int day, int hour, int minute = 0) {
  struct tm t = {};
  t.tm_year = 126; t.tm_mon = month - 1; t.tm_mday = day;
  t.tm_hour = hour; t.tm_min = minute;
  return watchyUtcTime(t, "UTC0");
}
static void check(time_t epoch, int day, int hour, int minute, int dst) {
  struct tm t;
  assert(watchyLocalTime(epoch, ROME, t));
  assert(t.tm_mday == day && t.tm_hour == hour && t.tm_min == minute);
  assert(t.tm_isdst == dst);
}
int main() {
  check(utc(1, 15, 12), 15, 13, 0, 0);
  check(utc(7, 15, 12), 15, 14, 0, 1);
  check(utc(3, 29, 0, 59), 29, 1, 59, 0);
  check(utc(3, 29, 1), 29, 3, 0, 1);
  check(utc(10, 25, 0, 59), 25, 2, 59, 1);
  check(utc(10, 25, 1), 25, 2, 0, 0);
  check(utc(12, 31, 23, 30), 1, 0, 30, 0);
  for (int month : {1, 7}) {
    time_t epoch = utc(month, 15, 12);
    struct tm local;
    assert(watchyLocalTime(epoch, ROME, local));
    assert(watchyUtcTime(local, ROME) == epoch);
  }
  struct tm local;
  assert(!watchyLocalTime(0, nullptr, local));
  assert(!watchyLocalTime(0, "", local));
  puts("Timezone tests passed: winter, summer, DST boundaries, rollover, manual setting.");
}
