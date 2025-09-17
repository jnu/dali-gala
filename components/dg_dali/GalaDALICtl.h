#pragma once
#include <Arduino.h>
#include "DALILib.h"

#define TX_PIN 17
#define RX_PIN 14

#ifdef __cplusplus
extern "C" {
#endif

bool GalaDALIInit(); 
int16_t* GalaDALIScanAllAddresses();
int16_t GalaDALICheckStatus(uint8_t addr);
void GalaDALIAllOn();
void GalaDALIAllOff();
void GalaDALICommission();

#ifdef __cplusplus
}
#endif