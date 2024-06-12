#ifndef THROTTLE_H
#define THROTTLE_H

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_SSD1306.h> // Hardware-specific library for SSD1306

#define THROTTLE_PIN  D0
#define NUM_READINGS  20  // Number of readings to average


extern Adafruit_SSD1306 display;
extern int throttle_readings[];
extern int read_index;
extern int total;
extern int average_throttle;
extern int old_throttle;
extern int raw_throttle_value;
extern int current_throttle_value;

void printAverageThrottle();

#endif // THROTTLE_H