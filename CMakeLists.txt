cmake_minimum_required(VERSION 3.28)

message( STATUS "Using CMake ${CMAKE_VERSION}" )

# Require out-of-source builds
file( TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH )

if ( EXISTS "${LOC_PATH}" )
    message( FATAL_ERROR "You cannot build in the source directory. Please use a build subdirectory." )
endif()

# Add paths to modules
list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/" )

# Turn on link time optimization for everything
set( CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON )

# Output compile commands to compile_commands.json (for debugging CMake issues)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# Enable versioning support for vcpkg.
#
# see: https://devblogs.microsoft.com/cppblog/take-control-of-your-vcpkg-dependencies-with-versioning-support/
set(VCPKG_FEATURE_FLAGS "versions")

if(APPLE)
    # Ensures that Apple Clang uses the latest feature
    # set instead of defaulting to c++98 or c++03.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
else()
    set(CMAKE_CXX_STANDARD 17)
endif()

# Build universal lib on macOS.
#
# Note that CMAKE_OSX_ARCHITECTURES must be set before project().
#
# This is important so that the built lib is compatible with both arm and x64 architectures for Godot builds.
# TODO: this causes issues with clion toolchain targets.. or does it?
# TODO: fix vcpkg only using one architecture
#   https://www.f-ax.de/dev/2022/11/09/how-to-use-vcpkg-with-universal-binaries-on-macos/
#   https://github.com/microsoft/vcpkg/discussions/19454
#
if ( APPLE )
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
            # Hack to make vcpkg build fat binaries for libraries.
            set(VCPKG_OVERLAY_TRIPLETS "${CMAKE_CURRENT_LIST_DIR}/vcpkg/triplets")
            set(VCPKG_TARGET_TRIPLET "universal-osx" CACHE STRING "target triplet" FORCE)

            # Tell the compiler to build a fat archive with both x86_64 and arm64 libraries.
            set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "")
        elseif()
            # TODO: see if forcing x86_54 on arm64 works as a patch.
            message(WARNING "Cannot build universal archive using non-Apple Clang.")
        endif()
    endif()
endif()

# Configure vcpkg if applicable.
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

if(CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg")
    message(STATUS "Target triplet for vcpkg is " ${VCPKG_TARGET_TRIPLET})
endif()

if (APPLE)
    # For some reason Apple clang++ doesn't search /usr/local/include automatically.
    message(STATUS "Adding /usr/local/include to header path to fix Vulkan SDK")

    # For some reason there's not a better way to inject the missing headers into
    # the vcpkg build process, and vcpkg ignores include_directories.
    set(ENV{CPLUS_INCLUDE_PATH} "/usr/local/include")
    set(ENV{C_INCLUDE_PATH} "/usr/local/include")
    message(STATUS "CPLUS_INCLUDE_PATH=$ENV{CPLUS_INCLUDE_PATH}")
    message(STATUS "C_INCLUDE_PATH=$ENV{C_INCLUDE_PATH}")
endif()




# Main project information
project( rive-godot
        LANGUAGES
            CXX C
        VERSION
            0.0.1
)

# Create our library
add_library( ${PROJECT_NAME} SHARED
        src/resources/rive_resource.cpp
        src/resources/rive_resource.h

        src/editor/plugin/editor_plugin.cpp
        src/editor/plugin/editor_plugin.h
        src/editor/plugin/screen/editor_main_screen.cpp
        src/editor/plugin/screen/editor_main_screen.h
        src/editor/plugin/inspector/resource/resource_editor_inspector_plugin.cpp
        src/editor/plugin/inspector/resource/resource_editor_inspector_plugin.h
        src/editor/plugin/inspector/resource/resource_inspector_preview_control.cpp
        src/editor/plugin/inspector/resource/resource_inspector_preview_control.cpp
        src/editor/plugin/inspector/resource/resource_inspector_preview_control.h
        src/editor/plugin/import/editor_import_plugin.cpp
        src/editor/plugin/import/editor_import_plugin.h
        src/editor/plugin/preview/editor_resource_preview_generator.cpp
        src/editor/plugin/preview/editor_resource_preview_generator.h
        src/editor/plugin/tooltip/editor_resource_tooltip_plugin.cpp
        src/editor/plugin/tooltip/editor_resource_tooltip_plugin.h

        src/runtime/rive_file.cpp
        src/runtime/rive_file.h
        src/runtime/rive_artboard.cpp
        src/runtime/rive_artboard.h
        src/runtime/rive_state_machine.cpp
        src/runtime/rive_state_machine.h
        src/runtime/rive_renderer.cpp
        src/runtime/rive_renderer.h
        src/runtime/rive_render_target.cpp
        src/runtime/rive_render_target.h
        src/scene/rive_control.cpp
        src/scene/rive_control.h


        src/gdextension.cpp
        src/runtime/rive_texture.cpp
        src/runtime/rive_texture.h
        src/runtime/rive_animation.cpp
        src/runtime/rive_animation.h
        src/resources/rive_drawable.cpp
        src/resources/rive_drawable.h
        src/editor/plugin/inspector/drawable/drawable_editor_inspector_plugin.cpp
        src/editor/plugin/inspector/drawable/drawable_editor_inspector_plugin.h
        src/editor/plugin/inspector/drawable/rive_drawable_property_editor_control.cpp
        src/editor/plugin/inspector/drawable/rive_drawable_property_editor_control.h
        src/editor/plugin/inspector/drawable/editor_resource_picker_rive_resource.cpp
        src/editor/plugin/inspector/drawable/editor_resource_picker_rive_resource.h
        src/editor/plugin/inspector/drawable/rive_drawable_picker_dialog.cpp
        src/editor/plugin/inspector/drawable/rive_drawable_picker_dialog.h

        src/runtime/renderers/skia/rive_renderer_skia.cpp
        src/runtime/renderers/skia/rive_renderer_skia.h
        src/runtime/renderers/skia/rive_render_target_skia.cpp
        src/runtime/renderers/skia/rive_render_target_skia.h
        src/runtime/renderers/skia/raster/render_target.cpp
        src/runtime/renderers/skia/raster/render_target.h
        src/runtime/renderers/skia/opengl/render_target.cpp
        src/runtime/renderers/skia/opengl/render_target.cpp
        src/runtime/renderers/skia/metal/render_target.cpp
        src/runtime/renderers/skia/metal/render_target.cpp
        src/runtime/renderers/skia/vulkan/render_target.cpp
        src/runtime/renderers/skia/vulkan/render_target.cpp

        src/runtime/renderers/pls/rive_renderer_pls.cpp
        src/runtime/renderers/pls/rive_renderer_pls.h
)

target_compile_features( ${PROJECT_NAME} PRIVATE cxx_std_17)

# LIB_ARCH is the architecture being built. It is set to the build system's architecture.
# For macOS, we build a universal library (both arm64 and x86_64).
set( LIB_ARCH ${CMAKE_SYSTEM_PROCESSOR} )
# TODO: this causes issues with clion toolchain targets.. or does it?
if ( APPLE )
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
            set( LIB_ARCH "universal" )
        elseif()
            message(STATUS "Using non-Apple Clang disables universal support, building for ${CMAKE_SYSTEM_PROCESSOR} instead.")
        endif()
    endif()
endif()

# LIB_DIR is where the actual library ends up. This is used in both the build directory and the
# install directory and needs to be consistent with the paths in the gdextension file.
#   e.g. linux.release.x86_64 = "lib/Linux-x86_64/libGDExtensionTemplate.so"
set( LIB_DIR "lib/${CMAKE_SYSTEM_NAME}-${LIB_ARCH}" )

message( STATUS "Building ${PROJECT_NAME} for ${LIB_ARCH} on ${CMAKE_SYSTEM_NAME}")

# BUILD_OUTPUT_DIR is where we put the resulting library (in the build directory)
set( BUILD_OUTPUT_DIR "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/" )

set_target_properties( ${PROJECT_NAME} PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN true
        RUNTIME_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/${LIB_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${BUILD_OUTPUT_DIR}/${LIB_DIR}"
)

if( NOT DEFINED CMAKE_DEBUG_POSTFIX )
    set_target_properties( ${PROJECT_NAME}
            PROPERTIES
            DEBUG_POSTFIX "-d"
    )
endif()

# Copy over additional files from the assets directory
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        ${BUILD_OUTPUT_DIR}
)

# Warnings
include( CompilerWarnings )

# Create and include version info file from git
include( GitVersionInfo )

target_include_directories( ${PROJECT_NAME}
        PRIVATE
        "src"
)

# Install library, extension file, and support files in ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
set( INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/" )

message( STATUS "Install directory: ${INSTALL_DIR}")

# Set the installation directory for the library depending on the platform and arch.
install( TARGETS ${PROJECT_NAME}
        LIBRARY
            DESTINATION ${INSTALL_DIR}/${LIB_DIR}
        RUNTIME
            DESTINATION ${INSTALL_DIR}/${LIB_DIR}
)

# Copy over support files
install( DIRECTORY "${CMAKE_SOURCE_DIR}/assets/"
        DESTINATION ${INSTALL_DIR}
        PATTERN ".*" EXCLUDE
)

# Build and copy templates for the .gdextension manifest.
add_subdirectory( templates )

# ccache
# Turns on ccache if found
include( ccache )

# Formatting
# Adds a custom target to format all the code at once
include( ClangFormat )

# Download and build godot-cpp with FetchContent.
include( godot-cpp )

## godot-cpp
## From here: https://github.com/godotengine/godot-cpp
#if ( NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/extern/godot-cpp/Makefile" )
#    message(
#            FATAL_ERROR
#            "[${PROJECT_NAME}] The godot-cpp submodule was not downloaded. Please update submodules: git submodule update --init --recursive."
#    )
#endif()

# Makes godot-cpp mark it's headers as SYSTEM to supress warning errors.
set( GODOT_CPP_SYSTEM_HEADERS ON CACHE BOOL "" FORCE )

#add_subdirectory( extern/godot-cpp )

set_target_properties( godot-cpp
        PROPERTIES
        CXX_VISIBILITY_PRESET hidden    # visibility needs to be the same as the main library
)

# Provided by vcpkg.
find_package(VulkanHeaders CONFIG)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Headers)

find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)

# Fetch the Rive C++ runtime and renderer for cmake.
include ( rive-cpp )
include ( rive-renderer )
#include ( glad )

# from vcpkg
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

#target_link_libraries(${PROJECT_NAME} PRIVATE glad)


target_link_libraries(${PROJECT_NAME} PRIVATE rive-skia)

add_dependencies(${PROJECT_NAME} godot-cpp rive-cpp rive-renderer)

target_link_libraries( ${PROJECT_NAME}
        PRIVATE
        godot-cpp

        # Statically link the Rive runtime and renderer with the extension.
        rive-cpp rive-renderer
)

if(APPLE)
    message(STATUS "Linking with Metal for macOS Skia Metal backend.")
    target_link_libraries(${PROJECT_NAME} PUBLIC
            "-framework CoreFoundation"
            "-framework QuartzCore"
            "-framework Metal")
    target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/src/runtime/renderers/skia/metal/metal-cpp)
endif()

# Use GLFW for creating cross-platform OpenGL and Vulkan contexts.
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)


add_subdirectory( demo )