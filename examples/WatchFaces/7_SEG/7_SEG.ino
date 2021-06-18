#include "Watchy_7_SEG.h"

Watchy7SEG watchy;

void setup() {
  // City names with spaces require a +
  // temp units must be either metric or imperial
  watchy.setWeatherLocation("New+York", "US", "metric");

  watchy.init();
}

void loop(){}



