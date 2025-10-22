#pragma once

#include "GalaDALICtl.h"
#include "GalaWebApp.h"
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <AsyncMessagePack.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

#ifdef __cplusplus
extern "C" {
#endif

bool GalaWebServerInit(void);

#ifdef __cplusplus
}
#endif