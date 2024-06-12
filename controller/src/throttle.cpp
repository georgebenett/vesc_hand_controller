#include "throttle.h"
#include <Arduino.h>
#include "battery.h"


void printAverageThrottle() {


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

    display.setCursor(0, 0);
    display.setTextColor(BLACK);
    display.print(old_throttle);
    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.print(average_throttle);
    old_throttle = average_throttle;
    display.display();
  }


}