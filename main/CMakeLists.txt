# Generate HTML header file from app.html
include(${COMPONENT_DIR}/generate_html_header.cmake)

idf_component_register(SRCS "main.cpp" "WS_WIFI.cpp" "WS_DALI.cpp" "DALI_Lib.cpp"
                       INCLUDE_DIRS "." "../build/include")
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23)