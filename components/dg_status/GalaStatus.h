#pragma once
#include "led_strip.h"

#ifdef __cplusplus
extern "C" {
#endif

bool GalaStatusSetWaiting();
bool GalaStatusSetReady();
bool GalaStatusSetError();
bool GalaStatusClear();
bool GalaStatusInit();

#ifdef __cplusplus
}
#endif