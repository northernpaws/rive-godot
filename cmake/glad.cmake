include(FetchContent)

FetchContent_Declare(
        glad-download
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        # NOTE: 3.x uses GDNative, 4.x uses GDExtension
        GIT_TAG glad2
)

FetchContent_GetProperties(glad-download)
if(NOT glad-download_POPULATED)
    FetchContent_Populate(glad-download)
endif()

add_subdirectory("${glad-download_SOURCE_DIR}/cmake")

# NOTE: We use OpenGL 3.3 for best compatability with hardware and software drivers.
message(STATUS "Building GLAD in ${glad-download_BINARY_DIR}")
glad_add_library(glad STATIC REPRODUCIBLE
        LOADER # gladLoaderLoadEGL
        LOCATION "${glad-download_BINARY_DIR}"
        API egl= gl:core=3.3
)

target_include_directories(glad INTERFACE "${glad-download_BINARY_DIR}/include")
