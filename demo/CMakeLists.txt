set( DEMO_DIR "${CMAKE_CURRENT_LIST_DIR}" )

# Install library and extension file in ${DEMO_DIR}/${PROJECT_NAME}
set( DEMO_INSTALL_DIR "${DEMO_DIR}/extensions/${PROJECT_NAME}/" )

# Register a custom target that copies the
# built libraries to the demo directory.
add_custom_target(demo
		COMMENT
		"Configuring the demo project directory."
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${BUILD_OUTPUT_DIR}" "${DEMO_INSTALL_DIR}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

set_target_properties(demo PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${DEMO_INSTALL_DIR}"
)

# Add the main library target as a dependency so the library
# is built before copying the libraries to the demo directory.
add_dependencies(demo ${PROJECT_NAME})
