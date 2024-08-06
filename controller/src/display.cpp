
#include <Arduino.h>
#include <arduino-timer.h>

#include "display.h"
#include "battery.h"
#include "throttle.h"
#include <arduino-timer.h>

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

extern bool connected;

int current_rpm = 0;
int old_rpm = 0;
int speed = 0;
int skateSoc = 0;
float old_voltage = 0;
float old_skate_soc = 0;
float adjustedVoltage = 0;


void printSpeed(int erpm) {
    if (erpm < 0) {
        erpm *= -1;
    }
    speed = (erpm * 60 * 2 * 3.14159) / 500000;
    tft.setCursor(10, 60);
    tft.print("speed: ");
    tft.setCursor(180, 60);
    tft.setTextColor(ST77XX_BLACK);
    tft.print(old_rpm);
    tft.setCursor(180, 60);
    tft.setTextColor(ST77XX_WHITE);
    tft.print(speed);
    old_rpm = speed;
}

void printVescVoltage(float voltage) {
    if (connected) {
        adjustedVoltage = incomingVoltage;
    }
    else
    adjustedVoltage = 0;
    tft.setCursor(10, 80);
    tft.print("vesc input: ");
    tft.setCursor(180, 80);
    tft.setTextColor(ST77XX_BLACK);
    tft.print(old_voltage);
    tft.setCursor(180, 80);
    tft.setTextColor(ST77XX_WHITE);
    //print aproximate voltage, 1 decimal case
    tft.print(adjustedVoltage, 1);
    old_voltage = adjustedVoltage;
}

void printSkateSoc() {

    if (connected) {
        skateSoc = map(incomingVoltage, 31, 42, 0, 100);
    }
    else
    skateSoc = 0;
    tft.setCursor(10, 100);
    tft.print("skate soc: ");
    tft.setCursor(180, 100);
    tft.setTextColor(ST77XX_BLACK);
    tft.print(old_skate_soc);
    tft.setCursor(180, 100);
    tft.setTextColor(ST77XX_WHITE);
    tft.print(skateSoc);
    old_skate_soc = skateSoc;
}

void printTripDistance() {
    tft.setCursor(10, 120);
    tft.print("trip: ");
    tft.setCursor(180, 120);
    tft.setTextColor(ST77XX_BLACK);
    tft.print("0");
    tft.setCursor(180, 120);
    tft.setTextColor(ST77XX_WHITE);
    tft.print("0");
}
