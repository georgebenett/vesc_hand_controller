#ifndef THROTTLE_H
#define THROTTLE_H


#define THROTTLE_PIN  D0
#define NUM_READINGS  15  // Number of readings to average



extern int throttle_readings[];
extern int read_index;
extern int total;
extern int average_throttle;
extern int old_throttle;
extern int raw_throttle_value;
extern int current_throttle_value;

extern int raw_throttle_min;
extern int raw_throttle_max;

void getAverageThrottle();

#endif // THROTTLE_H