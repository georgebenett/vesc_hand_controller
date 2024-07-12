
#include <Arduino.h>
#include <arduino-timer.h>

#include "display.h"
#include "battery.h"
#include "throttle.h"
#include <arduino-timer.h>

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

int current_rpm = 0;
int old_rpm = 0;
int speed = 0;


void printSpeed(int erpm) {
    speed = (erpm * 60 * 2 * 3.14159) / 500000;
    tft.setCursor(10, 80);
    tft.print("Speed: ");
    tft.setCursor(180, 80);
    tft.setTextColor(ST77XX_BLACK);
    tft.print(old_rpm);
    tft.setCursor(180, 80);
    tft.setTextColor(ST77XX_WHITE);
    tft.print(speed);
    old_rpm = speed;
}
