#include "GalaDALICtl.h"
#include "esp_log.h"

static const char *TAG = "GalaDALICtl";

Dali dali;
int16_t DALI_Devices[64] = {0};
uint8_t DALI_ValidDevices = 0;

uint8_t bus_is_high() {
  return digitalRead(RX_PIN); // slow version
}

// use bus
void bus_set_low() {
  digitalWrite(TX_PIN, LOW); // opto slow version
}

// release bus
void bus_set_high() {
  digitalWrite(TX_PIN, HIGH); // opto slow version
}

void ARDUINO_ISR_ATTR onTimer() { dali.timer(); }

hw_timer_t *timer = NULL;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Check the status of a device on the DALI bus.
 *
 * @param addr The address of the device to check.
 * @return The status of the device.
 */
int16_t GalaDALICheckStatus(uint8_t addr) {
  return dali.cmd(DALI_QUERY_STATUS, addr);
}

/**
 * Initialize the DALI bus.
 *
 * This will initialize the DALI bus and do an initial scan to initialize the
 * address state.
 *
 * @return true if successful, false otherwise
 */
bool GalaDALIInit() {
  ESP_LOGI(TAG, "Initializing DALI bus\n");

  // setup RX/TX pin
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);

  timer = timerBegin(9600000);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0);

  dali.begin(bus_is_high, bus_set_high, bus_set_low);

  // Do an initial scan to initialize the address state.
  GalaDALIScanAllAddresses();

  return true;
}

/**
 * Turn all the lights on.
 */
void GalaDALIAllOn() {
  ESP_LOGI(TAG, "requesting all lights ON");
  dali.set_level(254);
}

/**
 * Turn all the lights off.
 */
void GalaDALIAllOff() {
  ESP_LOGI(TAG, "requesting all lights OFF");
  dali.set_level(0);
}

/**
 * Set the level of a device.
 *
 * @param level The level to set.
 * @param addr The address of the device to set.
 */
void GalaDALISetLevel(uint8_t addr, uint8_t level) {
  ESP_LOGI(TAG, "set level %d for device %d", level, addr);
  dali.set_level(level, addr);
}

/**
 * Set the color temperature of a device.
 *
 * @param addr The address of the device to set the temperature for.
 * @param value The temperature to set.
 * @return 0 on success
 */
uint8_t GalaDALISetTemp(uint8_t addr, uint16_t value) {
  ESP_LOGI(TAG, "set temp for device %d to %d", addr, value);
  return dali.set_temperature(addr, value);
}

/**
 * Execute a DALI command.
 *
 * @param cmd The command to execute.
 * @param arg The argument to the command.
 * @return The result of the command.
 */
int16_t GalaDALICmd(uint16_t cmd, uint8_t arg) {
  ESP_LOGI(TAG, "executing command %d with argument %d", cmd, arg);
  return dali.cmd(cmd, arg);
}

/**
 * Commission all devices on the DALI bus.
 */
void GalaDALICommission() {
  ESP_LOGI(TAG, "Commissioning all devices ...\n");
  uint8_t cnt =
      dali.commission(0xff); // init_arg=0b11111111 : all without short address
  ESP_LOGI(TAG, "DONE, assigned %d new short addresses\n", cnt);
  // TODO - Scan and assign again. This hasn't been tested yet.
}

/**
 * Scan all addresses on the DALI bus.
 *
 * @return The addresses and statuses of devices found.
 */
int16_t *GalaDALIScanAllAddresses() {
  ESP_LOGI(TAG, "Scanning for DALI devices\n");
  DALI_ValidDevices = 0;

  for (uint8_t addr = 0; addr < 64; addr++) {
    int16_t status = dali.cmd(DALI_QUERY_STATUS, addr);
    DALI_Devices[addr] = status;
    if (status & 0x01) {
      DALI_ValidDevices++;
    }

    delay(10);
  }
  ESP_LOGI(TAG, "Finished DALI scan: found %d / 64 devices\n",
           DALI_ValidDevices);

  return DALI_Devices;
}

/**
 * Get the current level of a device.
 *
 * @param addr The address of the device to get the level of.
 * @return The current level of the device.
 */
int16_t GalaDALIGetLevel(uint8_t addr) {
  return dali.get_level(addr);
}

/**
 * Get the current color temperature of a device.
 *
 * @param addr The address of the device to get the temperature of.
 * @return The current temperature of the device.
 */
int16_t GalaDALIGetTemp(uint8_t addr) {
  return dali.get_temperature(addr);
}

#ifdef __cplusplus
}
#endif