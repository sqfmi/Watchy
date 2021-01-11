// STARRY HORIZON for Watchy by SQFMI
// Copyright 2021 Dan Delany dan.delany@gmail.com
// Released under free MIT License : https://github.com/dandelany/watchy-faces/blob/main/LICENSE

#include <Watchy.h> //include the Watchy library
#include <Fonts/FreeSansBold9pt7b.h> //include any fonts you want to use
#include "MadeSunflower39pt7b.h"

#define STAR_COUNT 1300

const int horizonY = 145;
const int starCount = 150;

struct xyPoint {
  int x;
  int y;
};
struct Star {
  int x;
  int y;
  int r;
};

Star stars[STAR_COUNT];

void initStars() {
//  printf("init stars\n");
  // todo how to run this only once at startup? currently running every minute
  // use srand because randomSeed() doesnt work on ESP32 (not stable)
  srand(5287);
  for(int i = 0; i < STAR_COUNT; i++) {
    int starX = (rand() % 260) - 30;
    int starY = (rand() % 260) - 30;
    int radius = rand() % 2;
    if(i == 0) { printf("init %d %d\n", starX, starY); }

    if (i > STAR_COUNT * 0.99) radius = 2;
    else if (i > STAR_COUNT * 0.80) radius = 1;
    else radius = 0;
    stars[i] = { starX, starY, radius };
  }
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
          initStars();
        }
        void drawWatchFace(){ //override this method to customize how the watch face looks
          display.fillScreen(GxEPD_WHITE);
          display.fillRect(0,0, 200, horizonY, GxEPD_BLACK);
          drawTime();
          drawGrid();
          drawStars();
          drawDate();
        }
        void drawGrid() {
          int prevY = horizonY;
          for(int i = 0; i < 40; i+= 1) {
            int y = prevY + int(abs(sin(double(i) / 10) * 10));
            
            if(y <= 200) {
              display.drawLine(0, y, 200, y, GxEPD_BLACK);
            }
            prevY = y;
          }
        }
        void drawStars() {
          // draw field of stars
          // rotate stars so that they make an entire revolution once per hour
          int minute = (int)currentTime.Minute;
          double minuteAngle = ((2.0 * M_PI) / 60.0) * (double)minute;
//          printf("Minute %d\n", (int)currentTime.Minute);
//          printf("minuteAngle %f\n", minuteAngle);
            
          for(int starI = 0; starI < STAR_COUNT; starI++) {
            int starX = stars[starI].x;
            int starY = stars[starI].y;
            int starR = stars[starI].r;
//            if(starR >= 2) { printf("%d %d, %d \n", starX, starY, starR); }

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
          char* timeStr;
          asprintf(&timeStr, "%d:%02d\n", currentTime.Hour, currentTime.Minute);
          drawCenteredString(timeStr, 100, 115, false);
          free(timeStr);
        }

        void drawDate() {
          String monthStr = monthShortStr(currentTime.Month);
          String dayOfWeek = dayShortStr(currentTime.Wday);
          display.setFont(&FreeSansBold9pt7b);
          display.setTextColor(GxEPD_BLACK);
          char* dateStr;
          asprintf(&dateStr, "%s %s %d\n", dayOfWeek, monthStr, currentTime.Day);
          drawCenteredString(dateStr, 100, 198, true);
          free(dateStr);
        }

        void drawCenteredString(const String &str, int x, int y, bool drawBg) {
          int16_t x1, y1;
          uint16_t w, h;
          String myStr = str;
          display.getTextBounds(myStr, x, y, &x1, &y1, &w, &h);
//          printf("bounds: %d x %d y, %d x1 %d y1, %d w, %d h\n", 0, 100, x1, y1, w, h);
          display.setCursor(x - w / 2, y);
          if(drawBg) {
            int padY = 3;
            int padX = 10;
            display.fillRect(x - (w / 2 + padX), y - (h + padY), w + padX*2, h + padY*2, GxEPD_WHITE);
          }
//          display.drawRect(x - w / 2, y - h, w, h, GxEPD_WHITE);
          display.print(myStr);
        }
};

StarryHorizon face; //instantiate watchface

void setup() {
  face.init(); //call init in setup
}

void loop() {
  // this should never run, Watchy deep sleeps after init();
}
