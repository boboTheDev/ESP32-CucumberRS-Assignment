#include "power_saver_module.h"
#include "../wifi_module/wifi_module.h"
#include "../watchdog_module/watchdog_module.h"
#include <Arduino.h>
static uint64_t sleepDuration = 0;
int sleepAfter = 0;
// Declare the task handles from main.cpp as extern
extern TaskHandle_t clockDisplayHandle;
extern TaskHandle_t ntpSyncHandle;

void power_saver_init(int DEEP_SLEEP_DURATION = 10, int DEEP_SLEEP_AFTER = 10)
{
  sleepDuration = DEEP_SLEEP_DURATION * 1000000; // into microseconds
  sleepAfter = DEEP_SLEEP_AFTER * 1000;          // into milliseconds
  Serial.println("[Sleep]\t Initialized with duration (seconds): " + String(DEEP_SLEEP_DURATION));
  Serial.println("[Sleep]\t Sleep every (seconds): " + String(DEEP_SLEEP_AFTER));
}

void power_saver_start()
{
  Serial.println("[Sleep]\t Preparing to sleep...");
  Serial.println("[Sleep]\t Suspending tasks...");
  vTaskSuspend(clockDisplayHandle);
  vTaskSuspend(ntpSyncHandle);
  wifi_off();
  disable_WDT();

  Serial.println("[Sleep]\t Entering deep sleep for (seconds): " + String(sleepDuration / 1000000));
  esp_sleep_enable_timer_wakeup(sleepDuration);
  esp_deep_sleep_start();
}

void power_saver_task(void *parameter)
{
  while (true)
  {
    vTaskDelay(sleepAfter / portTICK_PERIOD_MS);
    power_saver_start();
  }
}