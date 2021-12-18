#include <Wire.h>
#include "AbstractRTC.h"
#include "DS3232.h"
#include "PCF8563.h"

#define RTC_DS_ADDR 0x68
#define RTC_PCF_ADDR 0x51

static bool _canConnectTo(int addr) {
  byte error;
  Wire.beginTransmission(addr);
  error = Wire.endTransmission(addr);
  return error == 0;
}

AbstractRTC* AbstractRTC::init() {
	if (_canConnectTo(RTC_DS_ADDR)) {
    return new DS3232();
  }

  if (_canConnectTo(RTC_PCF_ADDR)) {
    return new PCF8563();
  }

  return new AbstractRTC();
}

String AbstractRTC::getValue(String data, char separator, int index) {
	int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for (int i=0; i <= maxIndex && found <= index; i++) {
      if (data.charAt(i)==separator || i==maxIndex) {
          found++;
          strIndex[0] = strIndex[1] + 1;
          strIndex[1] = (i == maxIndex) ? i + 1 : i;
      }
  }

  if (found > index) {
      return data.substring(strIndex[0], strIndex[1]);
  }

  return "";
}