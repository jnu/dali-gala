idf_component_register(SRCS "GalaWebServer.cpp"
                       INCLUDE_DIRS "." 
                       REQUIRES  dg_webapp dg_dali esp_system arduino-esp32)
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23)