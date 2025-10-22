#include "GalaWebApp.h"
#include "esp_log.h"
#include "esp_system.h"

static const char *TAG = "GalaWebApp";

/**
 * Initialize the static assets used in the web application.
 *
 * This will mount the file system where the static assets are stored.
 *
 * @return true if successful, false otherwise
 */
bool GalaWebAppInit(void) {
  ESP_LOGI(TAG, "Mounting the web app image ...");
  bool lfs = LittleFS.begin(false, "/littlefs", 5, "storage");

  if (!lfs) {
    ESP_LOGI(TAG, "Failed to mount the web app image");
    return false;
  }

  return true;
}