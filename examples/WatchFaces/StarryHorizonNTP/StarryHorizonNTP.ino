// STARRY HORIZON for Watchy by SQFMI
// Copyright 2021 Dan Delany dan.delany@gmail.com
// Released under free MIT License : https://github.com/dandelany/watchy-faces/blob/main/LICENSE

#include <Watchy.h> //include the Watchy library
#include <Fonts/FreeSansBold9pt7b.h> //include any fonts you want to use
#include <TimeLib.h>
#include <WiFiUdp.h>


#include "MadeSunflower39pt7b.h"

#include "stars.h"

#define STAR_COUNT 900

static const char ntpServerName[] = "us.pool.ntp.org";
int timeZone = -5;  // Eastern Standard Time (USA)
unsigned int localPort = 8888;  // local port to listen for UDP packets


time_t getNtpTime();
void sendNTPpacket(IPAddress &address);


const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
WiFiUDP udp;

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (udp.parsePacket() > 0) ; // discard any previously received packets
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}



const int horizonY = 150;
const int planetR = 650;

struct xyPoint {
  int x;
  int y;
};

//Star myStars[STAR_COUNT];
void initStars() {
  // The random star field is loaded from PROGMEM - see stars.h
  // this is the function that was used to generate the stars in stars.h
  // it does not run normally but can be used to re-generate stars with different parameters
  // tweak as needed, call this function in init, open serial monitor, and paste the results into stars.h
  srand(5287); // randomSeed() is not stable on ESP32, use srand
  printf("const Star STARS[] PROGMEM = {\n");
  for(int i = 0; i < STAR_COUNT; i++) {
    int starX = (rand() % 260) - 30;
    int starY = (rand() % 260) - 30;
    int radius = 0;
    if (i > STAR_COUNT * 0.99) radius = 2;
    else if (i > STAR_COUNT * 0.80) radius = 1;
    printf("  { %d, %d, %d },\n", starX, starY, radius);
    //    myStars[i] = { starX, starY, radius };
  }
  printf("};\n");
}

struct xyPoint rotatePointAround(int x, int y, int ox, int oy, double angle) {
  // rotate X,Y point around given origin point by a given angle
  // based on https://gist.github.com/LyleScott/e36e08bfb23b1f87af68c9051f985302#file-rotate_2d_point-py-L38
  double qx = (double)ox + (cos(angle) * (double)(x - ox)) + (sin(angle) * (double)(y - oy));
  double qy = (double)oy + (-sin(angle) * (double)(x - ox)) + (cos(angle) * (double)(y - oy));
  struct xyPoint newPoint;
  newPoint.x = (int)qx;
  newPoint.y = (int)qy;
  return newPoint;
}

class StarryHorizon : public Watchy {
  public:
    StarryHorizon() {
      // uncomment to re-generate stars
      //          initStars();
    }
    void setTimeNTP(){
      guiState = APP_STATE;
      RTC.read(currentTime);
      pinMode(DOWN_BTN_PIN, INPUT);
      pinMode(UP_BTN_PIN, INPUT);
      pinMode(MENU_BTN_PIN, INPUT);  
      pinMode(BACK_BTN_PIN, INPUT);  
      display.init(0, true); //_initial_refresh to false to prevent full update on init
      display.setFullWindow();

      time_t t = 0;
      int packet_counter = 0;
      while(t == 0){
        display.setCursor(0, 0);
        display.setFont(&FreeMonoBold9pt7b);
        display.print("Connecting ... ");
        display.setCursor(0, 20);
        display.print("timezone is ");
        display.setCursor(0, 40);
        display.print(timeZone);
        display.print(" UTC");
        display.setCursor(0, 80);
        display.print(packet_counter);
        t = getNtpTime();
        packet_counter += 1;
        if(digitalRead(MENU_BTN_PIN) == 1){
          //user wants to fall back to manual input
          return Watchy::setTime(); //return to exit out completely
        }      
        if(digitalRead(BACK_BTN_PIN) == 1){
          //give up, user doesn't want to change time
          break; //break from the while loop (which will...reset time to zero unless we stop that)
        }      
        //TODO: set timezone with UP and DOWN
        display.display(true); //partial refresh
      }
      display.hibernate();
      if( t != 0 ){
        RTC.set(t);
      } //if t is zero, it wasn't set and shouldn't override whatever time we have
      showMenu(menuIndex, false);
    }
    void setTime(){ //need to add "virtual" (e.g. virtual void setTime) in the Watchy.h that gets compiled (~/Arduino/libraries/Watchy/src/...)
      //if we have wifi, call NTP version (which can fall back to manual mode)
      //if we don't, go straight to manual mode
      if(connectWiFi()){
        setTimeNTP();
      } else {
        Watchy::setTime();
      }
    }
    void drawWatchFace(){
      display.fillScreen(GxEPD_BLACK);
      display.fillCircle(100, horizonY + planetR, planetR, GxEPD_WHITE);
      drawGrid();
      drawStars(STARS);
      drawTime();
      drawDate();
    }
    void drawGrid() {
      int prevY = horizonY;
      for(int i = 0; i < 40; i+= 1) {
        int y = prevY + int(abs(sin(double(i) / 10) * 10));
        if(y <= 200) {
          display.drawFastHLine(0, y, 200, GxEPD_BLACK);
        }
        prevY = y;
      }
      int vanishY = horizonY - 25;
      for (int x = -230; x < 430; x += 20) {
        display.drawLine(x, 200, 100, vanishY, GxEPD_BLACK);
      }
    }
    void drawStars(const Star stars[]) {
      // draw field of stars
      // rotate stars so that they make an entire revolution once per hour
      int minute = (int)currentTime.Minute;
      double minuteAngle = ((2.0 * M_PI) / 60.0) * (double)minute;
      //          printf("Minute %d, angle %f\n", (int)currentTime.Minute, minuteAngle);

      for(int starI = 0; starI < STAR_COUNT; starI++) {
        int starX = stars[starI].x;
        int starY = stars[starI].y;
        int starR = stars[starI].r;

        struct xyPoint rotated = rotatePointAround(starX, starY, 100, 100, minuteAngle);
        if(rotated.x < 0 || rotated.y < 0 || rotated.x > 200 || rotated.y > horizonY) { 
          continue;
        }
        if(starR == 0) {
          display.drawPixel(rotated.x, rotated.y, GxEPD_WHITE);
        } else {
          display.fillCircle(rotated.x, rotated.y, starR, GxEPD_WHITE);
        }
      }
    }
    void drawTime() {
      display.setFont(&MADE_Sunflower_PERSONAL_USE39pt7b);
      display.setTextColor(GxEPD_WHITE);
      display.setTextWrap(false);
      char* timeStr;
      asprintf(&timeStr, "%d:%02d", currentTime.Hour, currentTime.Minute);
      drawCenteredString(timeStr, 100, 115, false);
      free(timeStr);
    }

    void drawDate() {
      String monthStr = monthShortStr(currentTime.Month);
      String dayOfWeek = dayShortStr(currentTime.Wday);
      display.setFont(&FreeSansBold9pt7b);
      display.setTextColor(GxEPD_WHITE);
      display.setTextWrap(false);
      char* dateStr;
      asprintf(&dateStr, "%s %s %d", dayOfWeek, monthStr, currentTime.Day);
      drawCenteredString(dateStr, 100, 140, true);
      free(dateStr);
    }

    void drawCenteredString(const String &str, int x, int y, bool drawBg) {
      int16_t x1, y1;
      uint16_t w, h;

      display.getTextBounds(str, x, y, &x1, &y1, &w, &h);
      //          printf("bounds: %d x %d y, %d x1 %d y1, %d w, %d h\n", 0, 100, x1, y1, w, h);
      display.setCursor(x - w / 2, y);
      if(drawBg) {
        int padY = 3;
        int padX = 10;
        display.fillRect(x - (w / 2 + padX), y - (h + padY), w + padX*2, h + padY*2, GxEPD_BLACK);
      }
      // uncomment to draw bounding box
      //          display.drawRect(x - w / 2, y - h, w, h, GxEPD_WHITE);
      display.print(str);
    }
};

StarryHorizon face; //instantiate watchface

void setup() {
  face.init(); //call init in setup
}

void loop() {
  // this should never run, Watchy deep sleeps after init();
}
