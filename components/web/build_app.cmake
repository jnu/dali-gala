# CMake script to build webapp and inline the source into a C++ header file.

# Get the component directory from the parent scope
get_filename_component(COMPONENT_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

# Check if the HTML file exists
if(NOT EXISTS "${COMPONENT_DIR}/gala-app/build/index.html")
    message(FATAL_ERROR "HTML file not found: ${COMPONENT_DIR}/gala-app/build/index.html")
endif()

# Read the HTML file
file(READ "${COMPONENT_DIR}/gala-app/build/index.html" HTML_CONTENT)

# Escape the HTML content for C++ string literal
string(REPLACE "\\" "\\\\" HTML_CONTENT "${HTML_CONTENT}")
string(REPLACE "\"" "\\\"" HTML_CONTENT "${HTML_CONTENT}")
string(REPLACE "\n" "\\n" HTML_CONTENT "${HTML_CONTENT}")
string(REPLACE "\r" "\\r" HTML_CONTENT "${HTML_CONTENT}")

# Generate the header file
file(WRITE "${COMPONENT_DIR}/webapp.h"
"// Auto-generated header file containing HTML content
// Generated from gala-app/build/index.html at build time

#ifndef WEBAPP_H
#define WEBAPP_H

const char* APP_HTML_CONTENT = \"${HTML_CONTENT}\";

#endif // WEBAPP_H
")

string(LENGTH "${HTML_CONTENT}" HTML_LENGTH)
message(STATUS "Generated webapp.h with ${HTML_LENGTH} characters of HTML content")