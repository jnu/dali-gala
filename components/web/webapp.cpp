#include "esp_system.h"
#include "esp_log.h"
#include "webapp.h"

static const char *TAG = "webapp";

bool webapp_init(void)
{
    ESP_LOGI(TAG, "Mounting the web app image ...");
    bool lfs = LittleFS.begin(false, "/littlefs", 5, "storage");
  
    if (!lfs) {
        ESP_LOGI(TAG, "Failed to mount the web app image");
        return false;
    }

    return true;
}