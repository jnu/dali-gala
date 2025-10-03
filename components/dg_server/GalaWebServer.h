#pragma once

#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <AsyncMessagePack.h>
#include <LittleFS.h>
#include "GalaWebApp.h"
#include "GalaDALICtl.h"

#ifdef __cplusplus
extern "C" {
#endif

bool GalaWebServerInit(void);

#ifdef __cplusplus
}
#endif