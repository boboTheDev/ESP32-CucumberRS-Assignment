#include <Arduino.h>

// custom modules
#include "../lib/wifi_module/wifi_module.h"

void setup()
{
    delay(2000);
    Serial.begin(115200);
    wifi_init();
    wifi_connect();
}

void loop()
{
    Serial.println("Hello world!");
    delay(2000000);
}