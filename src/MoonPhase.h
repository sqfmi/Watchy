#ifndef MoonPhase_h
#define MoonPhase_h

#include <time.h>

#define MOON_SYNODIC_PERIOD   29.530588853   // Period of moon cycle in days.
#define	MOON_SYNODIC_OFFSET   2451550.26     // Reference cycle offset in days.
#define MOON_DISTANCE_PERIOD  27.55454988    // Period of distance oscillation
#define MOON_DISTANCE_OFFSET  2451562.2
#define MOON_LATITUDE_PERIOD  27.212220817   // Latitude oscillation
#define	MOON_LATITUDE_OFFSET  2451565.2
#define MOON_LONGITUDE_PERIOD 27.321582241   // Longitude oscillation
#define	MOON_LONGITUDE_OFFSET 2451555.8

class MoonPhase {
  public:
    double jDate;
    double phase;	      // 0 - 1, 0.5 = full
    double age;		      // Age in days of current cycle
    double fraction;	      // Fraction of illuminated disk
    double distance;	      // Moon distance in earth radii
    double latitude;	      // Moon ecliptic latitude
    double longitude;	      // Moon ecliptic longitude
    const char *phaseName;    // New, Full, etc.
    const char *zodiacName;   // Constellation 

    MoonPhase();
    void calculate(time_t);

  private:
    double julianDate(time_t);
};
#endif
