include(FetchContent)

FetchContent_Declare(
        godot-cpp
        GIT_REPOSITORY https://github.com/godotengine/godot-cpp.git
        # NOTE: 3.x uses GDNative, 4.x uses GDExtension
        GIT_TAG godot-4.2.2-stable
)

FetchContent_MakeAvailable(godot-cpp)
