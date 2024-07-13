
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
float old_voltage = 0;


void printSpeed(int erpm) {
    speed = (erpm * 60 * 2 * 3.14159) / 500000;
    tft.setCursor(10, 80);
    tft.print("speed: ");
    tft.setCursor(180, 80);
    tft.setTextColor(ST77XX_BLACK);
    tft.print(old_rpm);
    tft.setCursor(180, 80);
    tft.setTextColor(ST77XX_WHITE);
    tft.print(speed);
    old_rpm = speed;
}

void printVescVoltage(float voltage) {
    tft.setCursor(10, 100);
    tft.print("vesc input: ");
    tft.setCursor(180, 100);
    tft.setTextColor(ST77XX_BLACK);
    tft.print(old_voltage);
    tft.setCursor(180, 100);
    tft.setTextColor(ST77XX_WHITE);
    //print aproximate voltage, 1 decimal case
    tft.print(incomingVoltage * 1.25 , 1);
    old_voltage = incomingVoltage;
}
