#pragma once
#include <WiFi.h>

// The name and password of the WiFi access point
#define APSSID        CONFIG_WIFI_SSID
#define APPSK         CONFIG_WIFI_PASSWORD

// Define the WiFi power save level
#if defined(CONFIG_WIFI_PS_LEVEL_NONE)
#define WIFI_PS_LEVEL WIFI_PS_NONE
#elif defined(CONFIG_WIFI_PS_LEVEL_LOW)
#define WIFI_PS_LEVEL WIFI_PS_MIN_MODEM
#elif defined(CONFIG_WIFI_PS_LEVEL_HIGH)
#define WIFI_PS_LEVEL WIFI_PS_MAX_MODEM
#endif

// Define the WiFi mode
#if defined(CONFIG_WIFI_MODE_AP)
#define WIFI_MODE WIFI_AP
#elif defined(CONFIG_WIFI_MODE_STA)
#define WIFI_MODE WIFI_STA
#endif

#ifdef __cplusplus
extern "C" {
#endif

bool GalaWiFiInit();

#ifdef __cplusplus
}
#endif