include(FetchContent)

option(RIVE_RENDERER_WITH_WEBGPU OFF)

FetchContent_Declare(
        rive-renderer-download
        GIT_REPOSITORY https://github.com/rive-app/rive-renderer.git
        # Latest working commit hash:
        #  https://github.com/rive-app/rive-renderer/commit/a6c995a7d23ecdbb87977da9af7e255d310bdff0
        GIT_TAG a6c995a7d23ecdbb87977da9af7e255d310bdff0
)

# Fetch the Rive renderer if necessary.
FetchContent_GetProperties(rive-renderer-download)
if(NOT rive-download_POPULATED)
    FetchContent_Populate(rive-renderer-download)
endif()

# provided by vcpkg
#find_package(OpenGL REQUIRED
#        COMPONENTS OpenGL EGL
#)

# Disable for this list file to ignore some warnings from the renderer sources.
set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)

# Create a library for building the Rive library sources.
#
# We where using ExternalProject_Add and running premake5 and make,
# but the resulting library was causing linking errors that where
# not straightforward to resolve due to the premake5 generated
# makefiles calling ar for linking instead of using the clang linker.
add_library(rive-renderer STATIC
        # for i in renderer/**/*.cpp; do [ -f "$i" ] && echo "\${rive-renderer-download_SOURCE_DIR}/$i"; done
        ${rive-renderer-download_SOURCE_DIR}/renderer/gr_triangulator.cpp
        ${rive-renderer-download_SOURCE_DIR}/renderer/intersection_board.cpp
        ${rive-renderer-download_SOURCE_DIR}/renderer/path_utils.cpp
        ${rive-renderer-download_SOURCE_DIR}/renderer/pls.cpp
        ${rive-renderer-download_SOURCE_DIR}/renderer/pls_draw.cpp
        ${rive-renderer-download_SOURCE_DIR}/renderer/pls_factory.cpp
        ${rive-renderer-download_SOURCE_DIR}/renderer/pls_image.cpp
        ${rive-renderer-download_SOURCE_DIR}/renderer/pls_paint.cpp
        ${rive-renderer-download_SOURCE_DIR}/renderer/pls_path.cpp
        ${rive-renderer-download_SOURCE_DIR}/renderer/pls_render_context.cpp
        ${rive-renderer-download_SOURCE_DIR}/renderer/pls_render_context_helper_impl.cpp
        ${rive-renderer-download_SOURCE_DIR}/renderer/pls_renderer.cpp
)

# Fix for -Wignored-qualifiers throw by the compiler for the renderer.
set_target_properties(rive-renderer PROPERTIES
        COMPILE_WARNING_AS_ERROR OFF
)

# TODO: place compiled files in ${rive-renderer-download_BUILD_DIR} instead of source dir.

if(NOT IOS)
    target_sources(rive-renderer PRIVATE
            ${rive-renderer-download_SOURCE_DIR}/renderer/gl/gl_state.cpp
            ${rive-renderer-download_SOURCE_DIR}/renderer/gl/gl_utils.cpp
            ${rive-renderer-download_SOURCE_DIR}/renderer/gl/load_store_actions_ext.cpp
            ${rive-renderer-download_SOURCE_DIR}/renderer/gl/pls_render_buffer_gl_impl.cpp
            ${rive-renderer-download_SOURCE_DIR}/renderer/gl/pls_render_context_gl_impl.cpp
            ${rive-renderer-download_SOURCE_DIR}/renderer/gl/pls_render_target_gl.cpp
    )
endif()

if (WIN32 OR APPLE OR UNIX)
    target_sources(rive-renderer PRIVATE
            ${rive-renderer-download_SOURCE_DIR}/renderer/gl/pls_impl_webgl.cpp
            ${rive-renderer-download_SOURCE_DIR}/renderer/gl/pls_impl_rw_texture.cpp

            ${rive-renderer-download_SOURCE_DIR}/glad/glad.c
            ${rive-renderer-download_SOURCE_DIR}/glad/glad_custom.c
    )
endif()

if(ANDROID)
    target_sources(rive-renderer PRIVATE
            ${rive-renderer-download_SOURCE_DIR}/renderer/gl/load_gles_extensions.cpp
            ${rive-renderer-download_SOURCE_DIR}/renderer/gl/pls_impl_ext_native.cpp
            ${rive-renderer-download_SOURCE_DIR}/renderer/gl/pls_impl_framebuffer_fetch.cpp
    )
endif()

if (WIN32)
    target_sources(rive-renderer PRIVATE
            ${rive-renderer-download_SOURCE_DIR}/renderer/d3d/pls_render_context_d3d_impl.cpp
    )
endif()

if(EMSCRIPTEN)
    target_sources(rive-renderer PRIVATE
            ${rive-renderer-download_SOURCE_DIR}/renderer/gl/pls_impl_webgl.cpp
    )
endif()

if (RIVE_RENDERER_WITH_WEBGPU)
    target_sources(rive-renderer PRIVATE
            ${rive-renderer-download_SOURCE_DIR}/renderer/webgpu/em_js_handle.cpp
            ${rive-renderer-download_SOURCE_DIR}/renderer/webgpu/pls_render_context_webgpu_impl.cpp
            ${rive-renderer-download_SOURCE_DIR}/renderer/webgpu/pls_render_context_webgpu_vulkan.cpp
    )
endif()

# TODO: might need to squash warning errors
#target_include_directories(rive-renderer INTERFACE SYSTEM
#        ${rive-renderer-download_SOURCE_DIR}/include
#        ${rive-renderer-download_SOURCE_DIR}/glad
#        ${rive-renderer-download_SOURCE_DIR}/renderer
#)

target_include_directories(rive-renderer SYSTEM PRIVATE
        ${rive-renderer-download_SOURCE_DIR}/include
        ${rive-renderer-download_SOURCE_DIR}/glad
        ${rive-renderer-download_SOURCE_DIR}/renderer
)

target_include_directories(rive-renderer SYSTEM PUBLIC
        ${rive-renderer-download_SOURCE_DIR}/include
        ${rive-renderer-download_SOURCE_DIR}/glad
)

target_link_libraries(rive-renderer PUBLIC rive-cpp)

if (WIN32 OR APPLE OR UNIX)
    target_compile_definitions(rive-renderer PUBLIC RIVE_DESKTOP_GL)
    message(STATUS "RIVE_DESKTOP_GL=1")
elseif(ANDROID)
    target_compile_definitions(rive-renderer PUBLIC RIVE_ANDROID)
    message(STATUS "RIVE_ANDROID=1")
elseif(IOS)
    target_compile_definitions(rive-renderer PUBLIC RIVE_IOS)
    message(STATUS "RIVE_IOS=1")
elseif(EMSCRIPTEN)
    target_compile_definitions(rive-renderer PUBLIC RIVE_WEBGL)
    message(STATUS "RIVE_WEBGL=1")
endif()

if (RIVE_RENDERER_WITH_WEBGPU)
    target_compile_definitions(rive-renderer PUBLIC RIVE_WEBGPU)
endif()

if(APPLE OR WIN32 OR UNIX)
    # TODO: is glad already bundeled with the renderer source in glad/?
#    find_package(glad CONFIG REQUIRED)
#    target_link_libraries(rive-renderer PRIVATE glad::glad)
#    message(STATUS "[rive-renderer] Using GLAD for platform-independent OpenGL loading")
endif()

# TODO: is the below blocks of target links required with glad?
if (WIN32)
#    find_package(glfw3 CONFIG REQUIRED)
#    find_package(OpenGL CONFIG REQUIRED)
#
#    target_link_libraries(rive-renderer PRIVATE glfw OpenGL::GL)
#
#    # Windows-provided libraries
#    target_link_libraries(rive-renderer PRIVATE
#            d3d11.lib dxgi.lib d3dcompiler.lib
#    )
#
#    message(STATUS "[rive-renderer] Using GLFW3 Rive renderer on Windows")
elseif(APPLE)
    # TODO: are many of the below actually needed?

#    find_package(glfw3 CONFIG REQUIRED)
#    target_link_libraries(rive-renderer PRIVATE glfw)
#    message(STATUS "[rive-renderer] Using GLFW3 Rive renderer on Apple")
#
#    find_library(METAL Metal)
#    find_library(FOUNDATION Foundation)
#    find_library(QUARTZCORE QuartzCore)
#
#    # Apple-specific libraries
#    target_link_libraries(rive-renderer PRIVATE
#            "-framework Cocoa"
#            "-framework Metal"
#            "-framework Foundation"
#            "-framework QuartzCore"
#    )
elseif(UNIX)
    message(FATAL_ERROR "[rive-renderer] Not implemented yet")
elseif(EMSCRIPTEN)
    message(FATAL_ERROR "[rive-renderer] Not implemented yet")
else()
    # fallback to skia renderer?
endif()

# ====================
# Shader compilation

# For some reason it uses Python scripts in the makefile for compiling...
find_package(Python3 REQUIRED COMPONENTS Interpreter)

if( Python3_FOUND )
    # Check that the ply package is installed
    execute_process(
            COMMAND ${Python3_EXECUTABLE} -c "import ply"
            WORKING_DIRECTORY ${rive-renderer-download_SOURCE_DIR}/renderer/shaders
            RESULT_VARIABLE EXIT_CODE
            OUTPUT_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(EXIT_CODE STREQUAL "1")
        message(STATUS "[rive-renderer] python returned `${EXIT_CODE}`")
        message(STATUS "[rive-renderer] Rive renderer requires the `ply` Python package to be installed.")
        message(STATUS "[rive-renderer] Attempting to install using pip...")

        execute_process(
                COMMAND ${Python3_EXECUTABLE} -m pip install ply
                WORKING_DIRECTORY ${rive-renderer-download_SOURCE_DIR}/renderer/shaders
                RESULT_VARIABLE EXIT_CODE
                COMMAND_ECHO STDOUT
                OUTPUT_STRIP_TRAILING_WHITESPACE
        )

        if(EXIT_CODE STREQUAL "1")
            MESSAGE(FATAL_ERROR "[rive-renderer] Failed to install the `ply` package using pip.")
        endif ()
    else()
        message(STATUS "[rive-renderer] Found `ply` Python package.")
    endif()
elseif()
    MESSAGE(FATAL_ERROR "[rive-renderer] Python3 is required to compile Rive renderer shaders.")
endif()

option(RIVE_RENDERER_HUMAN_READABLE_SHADERS ON)

set(RIVE_SHADER_FLAGS "")
set(RIVE_SHADER_COMMAND "")

if(RIVE_RENDERER_HUMAN_READABLE_SHADERS)
    set(RIVE_SHADER_FLAGS "FLAGS=--human-readable")
endif()

if(APPLE)
    set(RIVE_SHADER_COMMAND "rive_pls_macosx_metallib")
elseif(IOS)
    set(RIVE_SHADER_COMMAND "rive_pls_ios_metallib")
elseif()
    set(RIVE_SHADER_COMMAND "minify")
endif()

add_custom_target(rive-renderer-shaders
        COMMAND make ${RIVE_SHADER_FLAGS} ${RIVE_SHADER_COMMAND}
        WORKING_DIRECTORY ${rive-renderer-download_SOURCE_DIR}/renderer/shaders
)

add_dependencies(rive-renderer rive-renderer-shaders)

