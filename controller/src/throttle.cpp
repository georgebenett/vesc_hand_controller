#include <Arduino.h>

#include "throttle.h"
#include "battery.h"
#include "display.h"

#define MAX_THROTTLE 255
#define MIN_THROTTLE 0

int raw_throttle_min = 4095;
int raw_throttle_max = 0;



/*TO-DO: define int sizes*/
int throttle_readings[NUM_READINGS];  // Array to store throttle readings
int read_index = 0;                   // Index of the current reading
int total = 0;                        // Running total of the readings
int average_throttle = 0;             // Average throttle value
int raw_throttle_value = 0;           // Raw throttle value
int old_throttle = 0;                 // Old throttle value
int current_throttle_value = 0;       // Current throttle value

void getAverageThrottle() {


  // Read the throttle value
  raw_throttle_value = analogRead(THROTTLE_PIN);

  //get the min and max throttle values for calibration
  if (raw_throttle_value < raw_throttle_min) {
    raw_throttle_min = raw_throttle_value;
  }
  if (raw_throttle_value > raw_throttle_max) {
    raw_throttle_max = raw_throttle_value;
  }
  // Map the throttle value to the range of 0-255
  current_throttle_value = map(raw_throttle_value, raw_throttle_max, raw_throttle_min, MAX_THROTTLE, MIN_THROTTLE);


  // Update the running total and readings array
  total = total - throttle_readings[read_index];
  throttle_readings[read_index] = current_throttle_value;
  total = total + current_throttle_value;

  // Advance to the next position in the array
  read_index = (read_index + 1) % NUM_READINGS;

  // Calculate the average
  average_throttle = (total / NUM_READINGS) ;
}