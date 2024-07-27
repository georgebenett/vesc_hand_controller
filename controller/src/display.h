#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <TFT_eSPI.h>


extern int incomingRPM;
extern float incomingVoltage;
extern float incomingCurrent;

void displayInit();
void getSpeed(int erpm);
void getVescVoltage(float voltage);
void displaySpeed();


#endif  // DISPLAY_H
