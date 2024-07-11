#include "throttle.h"
#include <Arduino.h>
#include "battery.h"
#include "display.h"

/*TO-DO: define int sizes*/
int throttle_readings[NUM_READINGS];  // Array to store throttle readings
int read_index = 0;                   // Index of the current reading
int total = 0;                        // Running total of the readings
int average_throttle = 0;             // Average throttle value
int raw_throttle_value = 0;           // Raw throttle value
int old_throttle = 0;                 // Old throttle value
int current_throttle_value = 0;       // Current throttle value

void printAverageThrottle() {

  tft.setCursor(10, 20);
  tft.print("throttle: ");
  // Read the throttle value
  raw_throttle_value = analogRead(THROTTLE_PIN);
  current_throttle_value = map(raw_throttle_value, 3800, 1100, 0, 180);


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