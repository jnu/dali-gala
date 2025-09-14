# CMake script to generate HTML header file from app.html
# This script reads app.html and generates a C++ header file with the HTML content as a string literal

# Read the HTML file
file(READ "${COMPONENT_DIR}/app.html" HTML_CONTENT)

# Escape the HTML content for C++ string literal
string(REPLACE "\\" "\\\\" HTML_CONTENT "${HTML_CONTENT}")
string(REPLACE "\"" "\\\"" HTML_CONTENT "${HTML_CONTENT}")
string(REPLACE "\n" "\\n" HTML_CONTENT "${HTML_CONTENT}")
string(REPLACE "\r" "\\r" HTML_CONTENT "${HTML_CONTENT}")

# Generate the header file
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/include/app_html.h"
"// Auto-generated header file containing HTML content
// Generated from app.html at build time

#ifndef APP_HTML_H
#define APP_HTML_H

const char* APP_HTML_CONTENT = \"${HTML_CONTENT}\";

#endif // APP_HTML_H
")
# Add the generated header to the include directories
#target_include_directories(main PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include)


