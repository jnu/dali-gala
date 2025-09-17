/**
 * GALA-DALI
 *
 * A simple, low-power, DALI controller for esp32-s3.
 */

#include "esp_log.h"

#include "GalaDALICtl.h"
#include "GalaWebServer.h"
#include "GalaWiFi.h"
#include "GalaStatus.h"

static const char *TAG = "GalaMain";

extern "C" void app_main()
{
    ESP_LOGI(TAG, "Setting up the device ...");
    if (!GalaStatusInit()) {
        ESP_LOGE(TAG, "The Status initialization failed!\r\n");
        return;
    }
    ESP_LOGI(TAG, "The Status initialization is complete!\r\n");

    if (!GalaWiFiInit()) {
        ESP_LOGE(TAG, "The WIFI initialization failed!\r\n");
        return;
    }
    ESP_LOGI(TAG, "The WIFI initialization is complete!\r\n");

    if (!GalaWebServerInit()) {
        ESP_LOGE(TAG, "The Web Server initialization failed!\r\n");
        return;
    }
    ESP_LOGI(TAG, "The Web Server initialization is complete!\r\n");
    
    if (!GalaDALIInit()) {
        ESP_LOGE(TAG, "The DALI initialization failed!\r\n");
        return;
    }
    ESP_LOGI(TAG, "DALI device scan is complete.\r\n");

    // Everything started up successfully!! woohoo!
    ESP_LOGI(TAG, "Device is ready to use!\r\n");
    GalaStatusSetReady();

    while (1) {
        // not needed
        delay(1000);
    }
}
