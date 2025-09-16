#pragma once
#include <WiFi.h>
#include "WS_DALI.h"
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <AsyncMessagePack.h>

// The name and password of the WiFi access point
#define APSSID "Gala"
#define APPSK  "GalaDali2025"

#ifdef __cplusplus
extern "C" {
#endif

void handleRoot(AsyncWebServerRequest *request);
void handleLights(AsyncWebServerRequest *request, JsonVariant &json);
void handleAddressesQuery(AsyncWebServerRequest *request);
void handleAddressQuery(AsyncWebServerRequest *request);

void sendError(AsyncWebServerRequest *request, int code, String message);

void WIFI_Init();

#ifdef __cplusplus
}
#endif