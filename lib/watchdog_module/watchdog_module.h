#ifndef watchdog_module_h
#define watchdog_module_h
#include <Arduino.h>

void watchdog_init(int WDT_TIMEOUT);
void disable_WDT();
void reset_WDT();

#endif