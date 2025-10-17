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
void GalaDALISetLevel(uint8_t addr, uint8_t level);
void GalaDALICommission();
int16_t GalaDALICmd(uint16_t cmd, uint8_t arg);
void GalaDALISetDTR(uint8_t dtr, uint8_t addr, uint8_t value);

#ifdef __cplusplus
}
#endif