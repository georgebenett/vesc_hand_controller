#ifndef BATTERY_H
#define BATTERY_H

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <arduino-timer.h>

#define BATTERY_PIN   D1
#define BAT_NUM_READINGS  40  // Number of readings to average

#define VBAT_TIMER_INTERVAL 1000
#define SOC_TIMER_INTERVAL  10000

#define battery_coeficient 2.05
#define BATTERY_CHARGE_MIN 3200
#define BATTERY_CHARGE_MAX 4200

extern Adafruit_ST7789 tft;

extern int raw_battery_voltage;
extern int battery_voltage;
extern int old_battery_voltage;
extern int battery_charge;
extern int old_battery_charge;

void printBatteryVoltage();
void printBatteryCharge();

#endif // BATTERY_H