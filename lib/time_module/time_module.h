#ifndef time_module_h
#define time_module_h
#include <Arduino.h>

void clock_init(long timezone, int dls_offset);
void displayClock(void *parameter);
void ntpSync(void *parameter);

#endif