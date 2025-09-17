idf_component_register(SRCS "main.cpp" "WS_WIFI.cpp" "WS_DALI.cpp" "DALI_Lib.cpp"
                       INCLUDE_DIRS "." 
                       REQUIRES web nvs_flash esp_wifi esp_http_server freertos driver led_strip esp_system esp_timer)
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23)