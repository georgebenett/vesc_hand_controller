#ifndef BATTERY_H
#define BATTERY_H


#include <arduino-timer.h>

#define BATTERY_PIN   D1
#define BAT_NUM_READINGS  40  // Number of readings to average

#define SOC_TIMER_INTERVAL  1000

#define battery_coeficient 2.05
#define BATTERY_CHARGE_MIN 3200
#define BATTERY_CHARGE_MAX 4200


extern int raw_battery_voltage;
extern int battery_voltage;
extern int old_battery_voltage;
extern int battery_charge;
extern int old_battery_charge;

int getBatteryVoltage();
int getBatteryCharge();

#endif // BATTERY_H