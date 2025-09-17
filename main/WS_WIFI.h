#pragma once
#include <WiFi.h>
#include "DALICtl.h"
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <AsyncMessagePack.h>
#include <LittleFS.h>
#include "webapp.h"

// The name and password of the WiFi access point
#define APSSID "Gala"
#define APPSK  "GalaDali2025"

#ifdef __cplusplus
extern "C" {
#endif

void handleLights(AsyncWebServerRequest *request, JsonVariant &json);
void handleAddressesQuery(AsyncWebServerRequest *request);
void handleAddressQuery(AsyncWebServerRequest *request);

void sendError(AsyncWebServerRequest *request, int code, String message);

void WIFI_Init();

#ifdef __cplusplus
}
#endif