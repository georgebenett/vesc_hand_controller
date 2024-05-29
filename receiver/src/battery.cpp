
#include <Arduino.h>
#include <arduino-timer.h>

#include "battery.h"
#include "throttle.h"
#include <arduino-timer.h>


void printBatteryVoltage() {

  tft.setCursor(35, 40);
  tft.print("battery voltage");
  // Read the battery voltage
  raw_battery_voltage = analogReadMilliVolts(BATTERY_PIN);
  battery_voltage = battery_coeficient * raw_battery_voltage;

  if (battery_voltage != old_battery_voltage) {
    tft.setCursor(100, 60);
    tft.setTextColor(ST77XX_BLACK);
    tft.print(old_battery_voltage);
    tft.setCursor(100, 60);
    tft.setTextColor(ST77XX_WHITE);
    tft.print(battery_voltage);
    old_battery_voltage = battery_voltage;
  }
}

void printBatteryCharge() {

  /*TO-DO: implement a coulomb counter instead of this*/
  battery_charge = map(battery_voltage, BATTERY_CHARGE_MIN, BATTERY_CHARGE_MAX, 0, 100);

  tft.setCursor(35, 80);
  tft.print("battery charge");
  tft.setCursor(110, 100);
  tft.setTextColor(ST77XX_BLACK);
  tft.print(old_battery_charge);
  tft.setCursor(110, 100);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(battery_charge);
  old_battery_charge = battery_charge;
}