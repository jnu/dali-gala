#pragma once

#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <AsyncMessagePack.h>
#include <LittleFS.h>
#include "WebApp.h"
#include "DALICtl.h"

#define WEB_SERVER_PORT CONFIG_WEB_SERVER_PORT

#ifdef __cplusplus
extern "C" {
#endif

bool WebServerInit(void);

#ifdef __cplusplus