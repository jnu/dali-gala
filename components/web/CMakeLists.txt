# Build the website into a single file and inline everything.

# Register the component with the generated header file
idf_component_register(INCLUDE_DIRS ".")

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
        OUTPUT ${COMPONENT_DIR}/webapp.h
        COMMAND ${CMAKE_COMMAND} -P ${COMPONENT_DIR}/build_app.cmake
        DEPENDS ${COMPONENT_DIR}/gala-app/build/index.html
        COMMENT "Generating webapp.h from built HTML"
        VERBATIM
    )

    # Create a target that depends on the header file
    add_custom_target(webapp_build
        DEPENDS ${COMPONENT_DIR}/webapp.h
    )

    # Make sure the component library depends on the webapp build
    add_dependencies(${COMPONENT_LIB} webapp_build)

    # Add the generated header to clean files
    set_property(DIRECTORY "${COMPONENT_DIR}" APPEND PROPERTY
        ADDITIONAL_CLEAN_FILES webapp.h)
endif()

