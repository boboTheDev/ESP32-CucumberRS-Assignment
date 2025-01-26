#include "time_module.h"
#include <HardwareSerial.h>
#include <time.h>
#include "../watchdog_module/watchdog_module.h"
#include "../wifi_module/wifi_module.h"
#include <esp_task_wdt.h>

const char *ntpServer = "pool.ntp.org";
long gmtOffset_sec = 25200; // UTC+7 (BKK)
int daylightOffset_sec = 0; // no dls

void clock_init(long timezone, int dls_offset)
{
  gmtOffset_sec = timezone;
  daylightOffset_sec = dls_offset;
}

void displayClock(void *parameter)
{
  int counter = 0; // for resetting wdt
  // Register this task with the watchdog timer
  esp_err_t result = esp_task_wdt_add(NULL);
  if (result == ESP_OK)
  {
    Serial.println("[Clock]\t Task registered with WDT.");
  }
  else if (result == ESP_ERR_INVALID_STATE)
  {
    Serial.println("[Clock]\t WDT is not initialized or already registered.");
  }
  else
  {
    Serial.println("[Clock]\t Failed to register task with WDT.");
  }
  while (true)
  {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
      Serial.println("[Clock]\t Failed to obtain time");
    }
    else
    {
      char timeStr[64];
      strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
      Serial.printf("[Clock]\t Local Time: %s\n", timeStr);
    }

    // WDT reset
    counter++;
    if (counter >= 5)
    {
      reset_WDT();
      counter = 0; // Reset the counter
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void ntpSync(void *parameter)
{

  while (true)
  {
    wifi_on();
    Serial.println("[NTP]\t Synchronizing time...");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
      Serial.println("[NTP]\t Failed to synchronize with NTP server.");
    }
    else
    {
      Serial.println("[NTP]\t Time successfully synchronized.");
      wifi_off();
    }
    vTaskDelay(60000 / portTICK_PERIOD_MS); // Wait 1 minute
  }
}