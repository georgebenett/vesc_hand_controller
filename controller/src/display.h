#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>

/*
#define TFT_SCL         D8
#define TFT_SDA         D10 just info */
#define TFT_CS          D4
#define TFT_RST         D6
#define TFT_DC          D5

extern int incomingRPM;
extern float incomingVoltage;
extern float incomingCurrent;

void printSpeed(int erpm);




#endif  // DISPLAY_H
