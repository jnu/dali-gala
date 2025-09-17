idf_component_register(SRCS "main.cpp" "WS_WIFI.cpp"
                       INCLUDE_DIRS "." 
                       REQUIRES web dali nvs_flash esp_wifi esp_http_server freertos driver led_strip esp_system)
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23)