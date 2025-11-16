#ifndef TIMEZONESGMT_H
#define TIMEZONESGMT_H

// This file is used to set up GMT based timezones.

// You don't need to change anything here to be able to set up GMT based time.
// If you set TIMEZONES_NON_GMT_OVERRIDE to 1 (as for get summer time and leaps),
// you must provide one location based timezone.
// 0: GMT, 1: Location timezone.

// Visit the link below.

namespace TimezonesGMT {

#ifndef TIMEZONES_NON_GMT_OVERRIDE
#define TIMEZONES_NON_GMT_OVERRIDE 0 // 0: GMT, 1: Location timezone.
#endif

#ifndef TIMEZONES_SELECTED
#define TIMEZONES_SELECTED 0         // If above is 0, value is index of array below.
#endif                               // Etc/GMT0

#define TIMEZONES_LENGTH 28

  typedef struct TZ {
    const char* location;
    const char* timezone;
  } TZ;


  // https://raw.githubusercontent.com/nayarsystems/posix_tz_db/master/zones.csv
  static TZ tz_override = {
    "Europe/Madrid",
    "CET-1CEST,M3.5.0,M10.5.0/3"
  }; 

  static TZ timeZones[TIMEZONES_LENGTH] = {
    {"Etc/GMT+0","GMT0"},         // 0
    {"Etc/GMT+1","<-01>1"},       // 1
    {"Etc/GMT+2","<-02>2"},       // 2
    {"Etc/GMT+3","<-03>3"},       // 3
    {"Etc/GMT+4","<-04>4"},       // 4
    {"Etc/GMT+5","<-05>5"},       // 5
    {"Etc/GMT+6","<-06>6"},       // 6
    {"Etc/GMT+7","<-07>7"},       // 7
    {"Etc/GMT+8","<-08>8"},       // 8
    {"Etc/GMT+9","<-09>9"},       // 9
    {"Etc/GMT+10","<-10>10"},     // 10
    {"Etc/GMT+11","<-11>11"},     // 11
    {"Etc/GMT+12","<-12>12"},     // 12
    {"Etc/GMT-0","GMT0"},         // 13
    {"Etc/GMT-1","<+01>-1"},      // 14
    {"Etc/GMT-2","<+02>-2"},      // 15
    {"Etc/GMT-3","<+03>-3"},      // 16
    {"Etc/GMT-4","<+04>-4"},      // 17
    {"Etc/GMT-5","<+05>-5"},      // 18
    {"Etc/GMT-6","<+06>-6"},      // 19
    {"Etc/GMT-7","<+07>-7"},      // 20
    {"Etc/GMT-8","<+08>-8"},      // 21
    {"Etc/GMT-9","<+09>-9"},      // 22
    {"Etc/GMT-10","<+10>-10"},    // 23
    {"Etc/GMT-11","<+11>-11"},    // 24
    {"Etc/GMT-12","<+12>-12"},    // 25
    {"Etc/GMT-13","<+13>-13"},    // 26
    {"Etc/GMT-14","<+14>-14"},    // 27
  };

  static long OFFSETS_SEC[TIMEZONES_LENGTH] = {
    0, 3600, 7200, 10800, 14400, 18000, 21600, 25200, 28800, 32400, 36000, 39600, 43200, 0, -3600, -7200, -10800, -14400, -18000, -21600, -25200, -28800, -32400, -36000, -39600, -43200, -46800, -50400
  };

  static int8_t tzIndex = TIMEZONES_SELECTED;
}
#endif //TIMEZONES_GMT_H