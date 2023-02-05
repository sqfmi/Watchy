#include "Watchy_Mario.h"
#define NUM_W 44
#define NUM_H 44
#define COIN_W 24
#define COIN_H 30
#define PIPE_W 42
#define PIPE_H 47
#define MARIO_W 56
#define MARIO_H 54
#define NUM_SPACING 4
#define COIN_SPACING 4
#define FLOOR_H 19
#define PIPE_PADDING DISPLAY_HEIGHT - FLOOR_H - PIPE_H
#define X_PADDING (DISPLAY_WIDTH - (4*NUM_W) - (3*NUM_SPACING))/2
#define Y_PADDING 2*COIN_SPACING+COIN_H


const unsigned char *numbers [10] = {mario0, mario1, mario2, mario3, mario4, mario5, mario6, mario7, mario8, mario9};

void WatchyMario::drawWatchFace(){
    display.fillScreen(GxEPD_WHITE);
    display.drawBitmap(0, 0, mariobg, DISPLAY_WIDTH, DISPLAY_HEIGHT, GxEPD_BLACK);

    int hour10 = currentTime.Hour/10;
    int hour01 = currentTime.Hour%10;
    int minute10 = currentTime.Minute/10;
    int minute01 = currentTime.Minute%10;

    int pos = 0;

    if(hour01 == 0 && minute10 == 0 && minute01 == 0){
        pos = 0;
    }
    else if(minute10 == 0 && minute01 == 0){
        pos = 1;
    }  
    else if(minute01 == 0){
        pos = 2;
    }else{
        pos = 3;
    }

    display.drawBitmap(X_PADDING + pos*(NUM_SPACING + NUM_W) + (NUM_W/2 - MARIO_W/2) + (pos < 2 ? 8 : -8), Y_PADDING+NUM_H + 4, pos < 2 ? mariomariol : mariomarior, MARIO_W, MARIO_H, GxEPD_BLACK); //mario        
    display.drawBitmap(X_PADDING + pos*(NUM_SPACING + NUM_W) + (NUM_W/2 - COIN_W/2), COIN_SPACING, mariocoin, COIN_W, COIN_H, GxEPD_BLACK); //coin
    
    if(pos == 0){
        display.drawBitmap(DISPLAY_WIDTH - 2*PIPE_W, PIPE_PADDING, mariopipe, PIPE_W, PIPE_H, GxEPD_BLACK); //pipe  
    }
    else if(pos == 1 || pos == 2){
        display.drawBitmap(X_PADDING, PIPE_PADDING, mariopipe, PIPE_W, PIPE_H, GxEPD_BLACK); //pipe
        display.drawBitmap(DISPLAY_WIDTH - PIPE_W - X_PADDING, PIPE_PADDING, mariopipe, PIPE_W, PIPE_H, GxEPD_BLACK); //pipe  
    }
    else{
        display.drawBitmap(2*PIPE_W, PIPE_PADDING, mariopipe, PIPE_W, PIPE_H, GxEPD_BLACK); //pipe  
    }    
        

    //Hour
    display.drawBitmap(X_PADDING, pos == 0 ? Y_PADDING : Y_PADDING + 20, numbers[hour10], NUM_W, NUM_H, GxEPD_BLACK); //first digit
    display.drawBitmap(X_PADDING+NUM_SPACING+NUM_W, pos == 1 ? Y_PADDING : Y_PADDING + 20, numbers[hour01], NUM_W, NUM_H, GxEPD_BLACK); //second digit

    //Minute
    display.drawBitmap(X_PADDING+2*(NUM_SPACING+NUM_W), pos == 2 ? Y_PADDING : Y_PADDING + 20, numbers[minute10], NUM_W, NUM_H, GxEPD_BLACK); //first digit
    display.drawBitmap(X_PADDING+3*(NUM_SPACING+NUM_W), pos == 3 ? Y_PADDING : Y_PADDING + 20, numbers[minute01], NUM_W, NUM_H, GxEPD_BLACK); //second digit   
}