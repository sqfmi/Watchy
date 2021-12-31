#include "Watchy_DOS.h"

void WatchyDOS::drawWatchFace(){
    char time[6];
    time[0] = '0' + ((currentTime.Hour/10)%10);
    time[1] = '0' + (currentTime.Hour%10);
    time[2] = ':';
    time[3] = '0' + ((currentTime.Minute/10)%10);
    time[4] = '0' + (currentTime.Minute%10);
    time[5] = 0;
    display.fillScreen(GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    display.setFont(&Px437_IBM_BIOS5pt7b);
    display.setCursor(0, 24);
    display.println("WATCHY-DOS 1.1.8");
    display.println("Copyright (c) 2020");
    display.println(" ");
    display.print("AUTOEXEC BAT ");
    display.println(time);
    display.print("COMMAND  COM ");
    display.println(time);
    display.print("CONFIG   SYS ");
    display.println(time);
    display.print("ESPTOOL  PY  ");
    display.println(time);
    display.println(" ");
    display.println("  4 files 563 bytes");
    display.println("  2048 bytes free");
    display.println(" ");
    display.println("<C:\\>esptool");
}