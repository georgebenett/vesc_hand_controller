
#include <Arduino.h>
#include <arduino-timer.h>

#include "battery.h"
#include "throttle.h"
#include "display.h"
#include <arduino-timer.h>

int raw_battery_voltage =  0;
int battery_voltage = 0;
int old_battery_voltage = 0;                  // Old battery voltage
int battery_charge = 0;
int old_battery_charge = 0;


int getBatteryVoltage() {


  raw_battery_voltage = analogReadMilliVolts(BATTERY_PIN);
  battery_voltage = battery_coeficient * raw_battery_voltage;

  if (battery_voltage < 0) {
    battery_voltage = 0;
  }
  if (battery_voltage > 4200) {
    battery_voltage = 4200;
  }
  return battery_voltage;

}

void printBatteryCharge() {

  /*TO-DO: implement a coulomb counter instead of this*/
  getBatteryVoltage();
  battery_charge = map(battery_voltage, BATTERY_CHARGE_MIN, BATTERY_CHARGE_MAX, 0, 100);

  tft.setCursor(10, 40);
  tft.print("battery soc: ");
  tft.setCursor(180, 40);
  tft.setTextColor(ST77XX_BLACK);
  tft.print(old_battery_charge);
  tft.setCursor(180, 40);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(battery_charge);
  old_battery_charge = battery_charge;
  tft.print("%");
}