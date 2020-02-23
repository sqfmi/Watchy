/*
 * Watchy - Accelerometer Example 
 * Prints out the BMA423 accelerometer data on Serial
 */

#ifndef ESP32
#error Please select ESP32 Wrover Module under Tools > Board
#endif

#include <bma.h>

BMA *bma = nullptr;
I2CBus *i2c = nullptr;

void setup()
{
  byte data;
  Serial.begin(115200);
  i2c = new I2CBus();
  bma = new BMA(*i2c);
  bma->begin();
  bma->enableAccel();
}

void loop(){
  Accel acc;
  bool res = bma->getAccel(acc);
  Serial.print(acc.x);
  Serial.print(" , ");
  Serial.print(acc.y);
  Serial.print(" , ");
  Serial.println(acc.z);
}