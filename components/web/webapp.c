#include "esp_system.h"
#include "esp_log.h"
#include "webapp.h"

static const char *TAG = "webapp";

bool webapp_init(void)
{
    ESP_LOGI(TAG, "Init");

    return true;
}