# Rive Godot

Work-in-progress GDExtension for loading and rendering Rive animation files in Godot.

Currently working with inefficient Skia-based renderers that copy pixel buffers from a separate render context into Godot's render context. Working towards integrating the Rive render natively with Godot.

Build instructions are currently lacking as the library is incomplete, but could be compiled by someone experienced with C++.

# Developing

## Debugging

> To enable debugging the library within Godot on **MacOS**, you'll need to modify the entitilements of the Godot editor to allow the `com.apple.security.get-task-allow` entitlement so that the debugger can attach to the process.
> ```bash
> $ codesign -s - --deep --force --options=runtime --entitlements ./editor.entitlements /Applications/Godot.app
> ```
> Note that this may cause a security signing warning the next time you start the Godot editor.

### CLion

To debug the extension using CLion, start the Godot editor, open your project, and then in CLion navigate to **Run > Attach to Process...** and select to Godot process.

To debug crashes on project load, my method is to have the **Attach to Process...** dialog open before I open the Godot editor with the search term "Godot" already entered in the process list, and then launch the editor, open the project, and quickly switch to CLion, press the refresh button on the process list, and then double-click on the Godot entry to attach the debugger before the project fully opens.

> Ideally we'd add a CLion configuration that builds the CMake library and then executes the Godot editor so that we can attach to the debugger immediately, but I've had some problems with getting Godot.app to correctly launch under CLion.


# Compiling

## MacOS

### Compiler

By default, the project will try to build a universal library for both arm64 and x86_64 Apple devices. This only works
correctly if the C++ compiler is using the XCode-bundled Clang and will fail with mach-o linker errors if LLVM is used.

The main CMake list attempts to detect for this use case and will disable universal builds in favour of building for
the current processor arch instead.

Macport's Clang MAY work, but needs to tested.

### Debugging

```
sudo DevToolsSecurity -enable
```

# TODO

 - Go through bugs here for ideas https://github.com/Zylann/godot_voxel/issues/442
 - Mac uses MESA for EGL support
 - Skia from vcpkg throws an error that the vulkan header can't be found unless the Vulkan SDK is installed on macOS
 - All Skia renderers EXCEPT Raster did not correctly draw a transparent background around the rendered object.
   - Probably an issue with our clear code.
 - The OpenGL renderer SHOULD be portable to webassembly fairly easily, but skia also has a webGPU moduel

# dependencies

  - ANGLE - EGL support for the OpenGL renderers' headless contexts

## macOS

 - Vulkan SDK - https://vulkan.lunarg.com/sdk/home#mac
   - bundled moltonvk is used for vulkan renderers