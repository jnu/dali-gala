# This component builds the web app static assets into a LittleFS image
# That will be flashed to the device and mounted by the application.

# Register the component with the generated header file
idf_component_register(SRCS "webapp.cpp" INCLUDE_DIRS "." REQUIRES esp_system arduino-esp32)

# Only run this during the build phase, not during early expansion
if(NOT CMAKE_BUILD_EARLY_EXPANSION)
    # First, build the Svelte app
    add_custom_command(
        OUTPUT ${COMPONENT_DIR}/gala-app/build/index.html
        COMMAND ${CMAKE_COMMAND} -E chdir ${COMPONENT_DIR}/gala-app pnpm build
        WORKING_DIRECTORY ${COMPONENT_DIR}/gala-app
        COMMENT "Building Svelte web app"
        VERBATIM
    )

    # Then, generate the header file from the built HTML
    add_custom_command(
        OUTPUT ${COMPONENT_DIR}/flash_data/app.html
        COMMAND cp ${COMPONENT_DIR}/gala-app/build/index.html ${COMPONENT_DIR}/flash_data/app.html
        DEPENDS ${COMPONENT_DIR}/gala-app/build/index.html
        COMMENT "Generating webapp.h from built HTML"
        VERBATIM
    )

    # Create a target that depends on the header file
    add_custom_target(webapp_build
        DEPENDS ${COMPONENT_DIR}/flash_data/app.html
    )

    # Make sure the component library depends on the webapp build
    add_dependencies(${COMPONENT_LIB} webapp_build)

    # Add the generated header to clean files
    set_property(DIRECTORY "${COMPONENT_DIR}" APPEND PROPERTY
        ADDITIONAL_CLEAN_FILES webapp.h)

    littlefs_create_partition_image(storage ./flash_data FLASH_IN_PROJECT)
endif()

