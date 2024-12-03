# ref: https://github.com/microsoft/vcpkg/discussions/19454#discussioncomment-8029796

if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(VCPKG_TARGET_ARCHITECTURE arm64)
    set(VCPKG_CRT_LINKAGE dynamic)
    set(VCPKG_LIBRARY_LINKAGE static)

    set(VCPKG_CMAKE_SYSTEM_NAME Darwin)

    # THIS WILL _NOT_ WORK FOR NONE CMAKE LIBS:
    set(VCPKG_OSX_ARCHITECTURES "arm64;x86_64")
elseif()
    message(FATAL_ERROR "Cannot build universal binaries without Apple Clang.")
    # TODO: may be possible to default to x86_64 instead and still link with arm thanks to rosetta?
# ref: https://github.com/microsoft/vcpkg/blob/master/triplets/x64-osx.cmake
#    set(VCPKG_TARGET_ARCHITECTURE x64)
#    set(VCPKG_CRT_LINKAGE dynamic)
#    set(VCPKG_LIBRARY_LINKAGE static)
#
#    set(VCPKG_CMAKE_SYSTEM_NAME Darwin)
#    set(VCPKG_OSX_ARCHITECTURES x86_64)
endif()
