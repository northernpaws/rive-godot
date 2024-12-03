# Rive Miniaudio Implementation
#
# Rive uses their own fork, see (ma_decoder_get_encoding_format):
#  - https://github.com/rive-app/miniaudio
#  - https://github.com/mackron/miniaudio/pull/794


# Fetch the Rive repo with FetchContent so that we can
# evaluate the include directory at configure time.
#
# see: https://discourse.cmake.org/t/can-you-combine-fetchcontent-and-externalproject/228/2
FetchContent_Declare(rive-miniaudio-download
        # Fetch the latest sources from the Rive Github repository.
        GIT_REPOSITORY "https://github.com/rive-app/miniaudio.git"
        GIT_TAG rive_changes_4
)

# Fetch the Rive content if necessary.
FetchContent_GetProperties(rive-miniaudio-download)
if(NOT rive-miniaudio-download_POPULATED)
    FetchContent_Populate(rive-miniaudio-download)
endif()

set(RIVE_MINIAUDIO_INCLUDE_DIRECTORY ${rive-miniaudio-download_SOURCE_DIR})
