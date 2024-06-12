
#include <Arduino.h>
#include <arduino-timer.h>

#include "battery.h"
#include "throttle.h"
#include <arduino-timer.h>


/*void printBatteryVoltage() {

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Read the battery voltage
  raw_battery_voltage = analogReadMilliVolts(BATTERY_PIN);
  battery_voltage = battery_coeficient * raw_battery_voltage;

  if (battery_voltage != old_battery_voltage) {
    tft.setCursor(180, 40);
    tft.setTextColor(ST77XX_BLACK);
    tft.print(old_battery_voltage);
    tft.setCursor(180, 40);
    tft.setTextColor(ST77XX_WHITE);
    tft.print(battery_voltage);
    old_battery_voltage = battery_voltage;
  }
}*/

/*void printBatteryCharge() {

  /*TO-DO: implement a coulomb counter instead of this
  battery_charge = map(battery_voltage, BATTERY_CHARGE_MIN, BATTERY_CHARGE_MAX, 0, 100);
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(120, 0);
  display.setTextColor(BLACK);
  display.print(old_battery_charge);
  display.setCursor(120, 0);
  display.setTextColor(WHITE);
  display.print(battery_charge);
  old_battery_charge = battery_charge;
  display.print("%");
  display.display();
}*/