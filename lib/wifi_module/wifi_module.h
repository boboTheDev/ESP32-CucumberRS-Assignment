#ifndef wifi_module_h
#define wifi_module_h
#include <Arduino.h>

void wifi_init(String ssid, String password);
void wifi_config();
void wifi_connect();
void wifi_off();
void wifi_on();

#endif