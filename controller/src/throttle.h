#ifndef THROTTLE_H
#define THROTTLE_H

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789

#define THROTTLE_PIN  D0
#define NUM_READINGS  15  // Number of readings to average


extern Adafruit_ST7789 tft;
extern int throttle_readings[];
extern int read_index;
extern int total;
extern int average_throttle;
extern int old_throttle;
extern int raw_throttle_value;
extern int current_throttle_value;

extern int raw_throttle_min;
extern int raw_throttle_max;

void printAverageThrottle();

#endif // THROTTLE_H