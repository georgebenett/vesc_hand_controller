
#include <Arduino.h>
#include <arduino-timer.h>

#include "display.h"
#include "battery.h"
#include "throttle.h"
#include <arduino-timer.h>

TFT_eSPI tft = TFT_eSPI();


int current_rpm = 0;
int old_rpm = 0;
int speed = 0;
int old_speed = 0;
float old_voltage = 0;
bool screenCleared = false;


void displayInit() {

    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(4);
}

void getSpeed(int erpm) {
    speed = ((erpm * 60 * 2 * 3.14159) / 500000);

    //invert speed if negative
    if (speed < 0) {
        speed *= -1;
    }
}

void getVescVoltage(float voltage) {
    float adjustedVoltage = incomingVoltage;

}

void displaySpeed() {

  if (speed < 10 && !screenCleared) {
    tft.fillScreen(TFT_BLACK);
    screenCleared = true;
  } else if (speed >= 10) {
    screenCleared = false;
  }

  tft.drawString(String(speed), 120, 150, 8);
}
