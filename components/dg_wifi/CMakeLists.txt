idf_component_register(SRCS "GalaWiFi.cpp"
                       INCLUDE_DIRS "." 
                       REQUIRES nvs_flash esp_wifi freertos esp_system)
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23)