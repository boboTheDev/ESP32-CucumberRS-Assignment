#include <Arduino.h>
#include <esp_sleep.h>

// custom modules
#include "../lib/wifi_module/wifi_module.h"
#include "../lib/time_module/time_module.h"
#include "../lib/watchdog_module/watchdog_module.h"
#include "../lib/power_saver_module/power_saver_module.h"

// Tasks
TaskHandle_t clockDisplayHandle;
TaskHandle_t ntpSyncHandle;
TaskHandle_t powerSaverHandle;

// Var
int WDT_TIMEOUT = 20;         // seconds
int DEEP_SLEEP_DURATION = 30; // seconds
int DEEP_SLEEP_AFTER = 80;    // seconds
int CPU_FREQUENCY = 80;
long TIMEZONE = 25200; // UTC+7 (BKK)
int DLS_OFFSET = 0;    // no dls
String ssid = "Auriol";
String password = "password1234";

void setup()
{
    delay(2000);
    setCpuFrequencyMhz(CPU_FREQUENCY);
    Serial.begin(115200);
    clock_init(TIMEZONE, DLS_OFFSET);
    wifi_init(ssid, password);
    // wifi_config();
    wifi_connect();

    watchdog_init(WDT_TIMEOUT);
    power_saver_init(DEEP_SLEEP_DURATION, DEEP_SLEEP_AFTER);

    // tasks
    xTaskCreatePinnedToCore(
        displayClock,
        "DisplayClock",
        2048,
        NULL,
        1,
        &clockDisplayHandle,
        0);
    delay(500);

    xTaskCreatePinnedToCore(
        ntpSync,
        "ntpSync",
        2048,
        NULL,
        2,
        &ntpSyncHandle,
        0);
    delay(500);

    xTaskCreatePinnedToCore(
        power_saver_task,
        "PowerSaver",
        2048,
        NULL,
        3,
        &powerSaverHandle,
        0);
    delay(500);
}

void loop()
{
}
