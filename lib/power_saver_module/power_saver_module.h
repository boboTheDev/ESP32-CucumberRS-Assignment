#ifndef power_saver_module_h
#define power_saver_module_h
#include <Arduino.h>
#include <esp_sleep.h>

void power_saver_init(int DEEP_SLEEP_DURATION, int DEEP_SLEEP_AFTER);
void power_saver_start();
void power_saver_task(void *parameter);

#endif