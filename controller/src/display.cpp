
#include <Arduino.h>
#include <arduino-timer.h>

#include "display.h"
#include "battery.h"
#include "throttle.h"
#include <arduino-timer.h>

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);