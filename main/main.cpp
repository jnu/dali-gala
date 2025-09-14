/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"
#include "WS_DALI.h"
#include "WS_WIFI.h"

static const char *TAG = "example";

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO

static led_strip_handle_t led_strip;

static void set_led_color(uint8_t r, uint8_t g, uint8_t b)
{
    /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color
     * NOTE -- args seem to be GREEN, RED, BLUE, not RGB.
     */
    led_strip_set_pixel(led_strip, 0, g, r, b);
    /* Refresh the strip to send data */
    led_strip_refresh(led_strip);
}

static void set_led_waiting(void)
{
    set_led_color(100, 84, 0);
}

static void set_led_ready(void)
{
    set_led_color(0, 100, 0);
}

static void clear_led(void)
{
    led_strip_clear(led_strip);
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink addressable LED!");
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {};
    strip_config.strip_gpio_num = BLINK_GPIO;
    strip_config.max_leds = 1; // at least one LED on board

    led_strip_spi_config_t spi_config = {};
    spi_config.spi_bus = SPI2_HOST;
    spi_config.flags = { .with_dma = true };
    ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, &led_strip));

    /* Set all LED off to clear all pixels */
    led_strip_clear(led_strip);
}

extern "C" void app_main()
{
    ESP_LOGI(TAG, "Setting up the device ...");
    
    configure_led();
    set_led_waiting();

    WIFI_Init();
    printf("The WIFI initialization is complete!\r\n");
  
    DALI_Init();
    printf("DALI device scan is complete.\r\n");

    printf("Device is ready to use!\r\n");
    set_led_ready();

    uint32_t Time_count=0;
    uint32_t Time=0;

    while (1) {
        WIFI_Loop();

        if(DALI_Loop && DALI_NUM){
            Time_count++;
            if(Time_count == 1)
            {
              Time_count = 0;
              Time ++;
              if(Time > 300){
                Time = 0;
              }
              setBrightness(Time%4 +1,Time/4);
            }
          }
        
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}
