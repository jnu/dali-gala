idf_component_register(SRCS "main.cpp"
                       INCLUDE_DIRS "." 
                       REQUIRES dg_webapp dg_dali dg_server dg_wifi nvs_flash esp_wifi esp_http_server freertos driver led_strip esp_system)
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23)