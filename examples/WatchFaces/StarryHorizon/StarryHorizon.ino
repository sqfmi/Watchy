// STARRY HORIZON for Watchy by SQFMI
// Copyright 2021 Dan Delany dan.delany@gmail.com
// Released under free MIT License : https://github.com/dandelany/watchy-faces/blob/main/LICENSE

#include <Watchy.h> //include the Watchy library
#include <Fonts/FreeSansBold9pt7b.h> //include any fonts you want to use
#include "MadeSunflower39pt7b.h"
#include "stars.h"
#include "settings.h"

#define STAR_COUNT 900

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
        StarryHorizon(const watchySettings& s) : Watchy(s) {
          // uncomment to re-generate stars
          // initStars();
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
          asprintf(&dateStr, "%s %s %d", dayOfWeek.c_str(), monthStr.c_str(), currentTime.Day);
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

StarryHorizon face(settings); //instantiate watchface

void setup() {
  face.init(); //call init in setup
}

void loop() {
  // this should never run, Watchy deep sleeps after init();
}
