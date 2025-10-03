#include "GalaWiFi.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include <stdarg.h>

static const char *TAG = "GalaWiFi";

// Configuration for the Wifi Access Point.
// Users can connect using this SSID, password, on this IP.
const char *ssid = APSSID;                
const char *password = APPSK;               
IPAddress apIP(10, 10, 0, 1);

char ipStr[16];

/**
 * Initialize the WIFI access point.
 *
 * This will initialize the WIFI access point and set up the IP address.
 *
 * @return true if successful, false otherwise
 */
bool initAP() {
  WiFi.mode(WIFI_AP);
  while(!WiFi.softAP(ssid, password)) {
    ESP_LOGE(TAG, "Soft AP creation failed.\r\n");
    ESP_LOGI(TAG, "Try setting up the WIFI again.\r\n");
    ESP_LOGI(TAG, "Tried to connect to %s with password %s\r\n", ssid, password);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  } 
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); // Set the IP address and gateway of the AP

  ESP_LOGI(TAG, "Setting up the AP ...");
  IPAddress myIP = WiFi.softAPIP();
  ESP_LOGI(TAG, "AP IP address: ");
  sprintf(ipStr, "%d.%d.%d.%d", myIP[0], myIP[1], myIP[2], myIP[3]);
  ESP_LOGI(TAG, "%s\r\n", ipStr);

  return true;
}

/**
 * Initialize the WIFI station.
 *
 * This will initialize the WIFI station and set up the IP address.
 *
 * @return true if successful, false otherwise
 */
bool initSTA() {
  WiFi.mode(WIFI_STA);
  ESP_LOGI(TAG, "Connecting to the WIFI ...");
  while(!WiFi.begin(ssid, password)) {
    ESP_LOGE(TAG, "Failed to connect to the WIFI.\r\n");
    ESP_LOGI(TAG, "Try setting up the WIFI again.\r\n");
    ESP_LOGI(TAG, "Tried to connect to %s with password %s\r\n", ssid, password);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
  return true;
}

/**
 * Initialize the WIFI access point.
 *
 * This will initialize the WIFI access point and set up the IP address.
 *
 * @return true if successful, false otherwise
 */
bool GalaWiFiInit()
{
  ESP_LOGI(TAG, "Initializing the WIFI ...");

  // Initialize NVS first (required for WiFi)
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  esp_wifi_set_ps(WIFI_PS_LEVEL);

  if (WIFI_MODE == WIFI_AP) {
    return initAP();
  } else if (WIFI_MODE == WIFI_STA) {
    return initSTA();
  } else {
    ESP_LOGE(TAG, "Invalid WiFi mode: %d", WIFI_MODE);
  }
  return false;
}
