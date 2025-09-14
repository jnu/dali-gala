#pragma once
#include <WiFi.h>
#include <WebServer.h>
#include "WS_DALI.h"

// The name and password of the WiFi access point
#define APSSID       "pp"
#define APPSK        "GoodPlaceGoodDog"
extern bool DALI_Loop;

#ifdef __cplusplus
extern "C" {
#endif

void setBrightness(int sliderId, int value);
void handleSwitch(uint8_t ledNumber);

void handleRoot();
void handleSetSlider();
void handleRGBOn();
void handleRGBOff();

void WIFI_Init();
void WIFI_Loop();

#ifdef __cplusplus
}
#endif