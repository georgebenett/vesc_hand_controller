#include "throttle.h"
#include <Arduino.h>
#include "battery.h"


void printAverageThrottle() {

  tft.setCursor(10, 20);
  tft.print("throttle: ");
  // Read the throttle value
  raw_throttle_value = analogRead(THROTTLE_PIN);
  current_throttle_value = map(raw_throttle_value, 0, 4095, -255, 255);

  // Update the running total and readings array
  total = total - throttle_readings[read_index];
  throttle_readings[read_index] = current_throttle_value;
  total = total + current_throttle_value;

  // Advance to the next position in the array
  read_index = (read_index + 1) % NUM_READINGS;

  // Calculate the average
  average_throttle = total / NUM_READINGS;

  if (average_throttle != old_throttle) {
    tft.setCursor(180, 20);
    tft.setTextColor(ST77XX_BLACK);
    tft.print(old_throttle);
    tft.setCursor(180, 20);
    tft.setTextColor(ST77XX_WHITE);
    tft.print(average_throttle);
    old_throttle = average_throttle;
  }


}