#include "Watchy_MacPaint.h"

const unsigned char *numbers [10] = {numbers0, numbers1, numbers2, numbers3, numbers4, numbers5, numbers6, numbers7, numbers8, numbers9};

void WatchyMacPaint::drawWatchFace(){
    display.fillScreen(GxEPD_WHITE);
    display.drawBitmap(0, 0, window, DISPLAY_WIDTH, DISPLAY_HEIGHT, GxEPD_BLACK);

    //Hour
    display.drawBitmap(35, 70, numbers[currentTime.Hour/10], 38, 50, GxEPD_BLACK); //first digit
    display.drawBitmap(70, 70, numbers[currentTime.Hour%10], 38, 50, GxEPD_BLACK); //second digit

    //Colon
    display.drawBitmap(100, 80, colon, 11, 31, GxEPD_BLACK); //second digit

    //Minute
    display.drawBitmap(115, 70, numbers[currentTime.Minute/10], 38, 50, GxEPD_BLACK); //first digit
    display.drawBitmap(153, 70, numbers[currentTime.Minute%10], 38, 50, GxEPD_BLACK); //second digit
}