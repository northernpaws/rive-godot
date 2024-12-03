FetchContent_Declare(rive-sheenbidi-download
        GIT_REPOSITORY "https://github.com/Tehreer/SheenBidi.git"
        GIT_TAG v2.6
)

FetchContent_GetProperties(rive-sheenbidi-download)
if(NOT rive-sheenbidi-download_POPULATED)
    FetchContent_Populate(rive-sheenbidi-download)
endif()
#
#add_library(rive-sheenbidi-debug STATIC
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/BidiChain.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/BidiTypeLookup.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/BracketQueue.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/GeneralCategoryLookup.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/IsolatingRun.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/LevelRun.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/PairingLookup.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/RunQueue.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/SBAlgorithm.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/SBBase.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/SBCodepointSequence.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/SBLine.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/SBLog.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/SBMirrorLocator.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/SBParagraph.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/SBScriptLocator.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/ScriptLookup.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/ScriptStack.c
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/StatusStack.c
#)
#
#set_target_properties(rive-sheenbidi-debug PROPERTIES LINKER_LANGUAGE CXX) #C
#
#set_target_properties(rive-sheenbidi-debug PROPERTIES
#        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${rive-sheenbidi-download_SOURCE_DIR}/Headers
#)
#
#target_include_directories(rive-sheenbidi-debug PUBLIC ${rive-sheenbidi-download_SOURCE_DIR}/Headers)
#
#add_library(rive-sheenbidi-release STATIC
#        ${rive-sheenbidi-download_SOURCE_DIR}/Source/SheenBidi.c
#)
#
#target_compile_definitions(rive-sheenbidi-release PUBLIC SB_CONFIG_UNITY)
#
#set_target_properties(rive-sheenbidi-release PROPERTIES LINKER_LANGUAGE CXX) #C
#
#set_target_properties(rive-sheenbidi-release PROPERTIES
#        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${rive-sheenbidi-download_SOURCE_DIR}/Headers
#)
#
#target_include_directories(rive-sheenbidi-release PUBLIC ${rive-sheenbidi-download_SOURCE_DIR}/Headers)

add_definitions(-DSB_CONFIG_UNITY)

message(STATUS "${rive-sheenbidi-download_SOURCE_DIR}/Source/SheenBidi.c")
add_library(rive-sheenbidi STATIC
        ${rive-sheenbidi-download_SOURCE_DIR}/Source/SheenBidi.c
)

target_compile_definitions(rive-sheenbidi PUBLIC SB_CONFIG_UNITY)

target_include_directories(rive-sheenbidi SYSTEM PUBLIC ${rive-sheenbidi-download_SOURCE_DIR}/Headers)
