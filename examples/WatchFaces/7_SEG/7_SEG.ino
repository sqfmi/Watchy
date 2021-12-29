#include "Watchy_7_SEG.h"

Watchy7SEG watchy;

void setup(){
  // initialise weather to New York City, Celsius temperature, 30-minute updates, English language, SQFMI's API key
  watchy.setupWeather(5128581, true, 30, String("en"), String("f058fe1cad2afe8e2ddc5d063a64cecb") );
  watchy.init();
}

void loop(){}
