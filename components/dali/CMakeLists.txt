idf_component_register(SRCS "DALICtl.cpp" "DALILib.cpp"
                       INCLUDE_DIRS "." 
                       REQUIRES freertos driver esp_system esp_timer arduino-esp32)
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23)