#pragma once
#include <WiFi.h>

// The name and password of the WiFi access point
#define APSSID CONFIG_WIFI_SSID
#define APPSK  CONFIG_WIFI_PASSWORD

#ifdef __cplusplus
extern "C" {
#endif

bool GalaWiFiInit();

#ifdef __cplusplus
}
#endif