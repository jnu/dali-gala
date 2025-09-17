idf_component_register(SRCS "GalaStatus.cpp"
                       INCLUDE_DIRS "." 
                       REQUIRES  led_strip esp_system)
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23)