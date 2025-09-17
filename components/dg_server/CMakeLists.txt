idf_component_register(SRCS "WebServer.cpp"
                       INCLUDE_DIRS "." 
                       REQUIRES  web dali esp_system arduino-esp32)
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23)