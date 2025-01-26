#include "watchdog_module.h"
#include <esp_task_wdt.h>
#include <Arduino.h>

extern TaskHandle_t clockDisplayHandle;

void watchdog_init(int WDT_TIMEOUT = 10)
{
  esp_err_t ESP32_ERROR;
  Serial.print("[WDT]\t Configured timeout (seconds):\t");
  Serial.println(WDT_TIMEOUT);
  esp_task_wdt_deinit();
  // WDT config

  ESP32_ERROR = esp_task_wdt_init(WDT_TIMEOUT, true); // panic if not reset

  if (ESP32_ERROR == ESP_OK)
  {
    Serial.println("[WDT]\t Watchdog Timer initialized successfully.");
  }
  else
  {
    Serial.print("[WDT]\t Failed to initialize WDT. Error: ");
    Serial.println(esp_err_to_name(ESP32_ERROR));
  }
  Serial.println("[WDT]\t Last Reset : " + String(esp_err_to_name(ESP32_ERROR)));
}

void disable_WDT()
{
  esp_task_wdt_delete(clockDisplayHandle);
  esp_task_wdt_deinit();
  Serial.println("[WDT]\t Watchdog Timer disabled.");
}

void reset_WDT()
{
  esp_task_wdt_reset();
  Serial.println("[WDT]\t Watchdog Timer reset.");
}