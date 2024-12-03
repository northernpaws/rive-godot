

#include <skia_factory.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/os.hpp>
#include <GLFW/glfw3.h>

#include "rive_renderer_skia.h"

#include "rive_render_target_skia.h"

#include "runtime/renderers/skia/raster/render_target.h"
//#include "runtime/renderers/skia/opengl/render_target.h"
#include "runtime/renderers/skia/metal/render_target.h"
#include "runtime/renderers/skia/opengl/render_target.h"
#include "runtime/renderers/skia/vulkan/render_target.h"

void RiveRendererSkia::_bind_methods() {

}

void RiveRendererSkia::_detect_backend() {
    auto platform = godot::OS::get_singleton()->get_name();

    godot::String rendering_method;

    // Try platform-specific rendering methods if applicable.
    if (platform == "Web") {
        rendering_method = godot::ProjectSettings::get_singleton()->get("rendering/renderer/rendering_method.web");
    } else if (platform == "Android" || platform == "iOS") {
        rendering_method = godot::ProjectSettings::get_singleton()->get("rendering/renderer/rendering_method.mobile");
    }

    // Fallback to the default rendering method otherwise.
    if (rendering_method.is_empty()){
        rendering_method = godot::ProjectSettings::get_singleton()->get("rendering/renderer/rendering_method");
    }

    // Retrieve the appropriate rendering driver based on the rendering method configured for the project.
    godot::String rendering_driver;
    if (rendering_method == "gl_compatibility") {
        // "opengl3", "opengl3_angle", "opengl3_es"
        rendering_driver = godot::ProjectSettings::get_singleton()->get("rendering/gl_compatibility/driver");
    } else if (rendering_method == "forward_plus" || rendering_method == "mobile") {
        // vulkan, d3d12
        rendering_driver = godot::ProjectSettings::get_singleton()->get("rendering/rendering_device/driver");
    }

    godot::UtilityFunctions::print("detected ", rendering_method, " driver ", rendering_driver);

    // Attempt to default to a Skia renderer that aligns with the engine rendering method and driver.
    //
    // Otherwise, default to a less performant CPU-based raster renderer.
    if (platform == "Web") {
        // Use a WebAssembly and WebGL renderer implementation.
        // TODO: implement
        //   https://www.reddit.com/r/godot/comments/1atvz4z/comment/kr0nyks/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
        ERR_PRINT("The WebGL Skia renderer for Rive is not yet implemented.");
    } else if (rendering_driver == "vulkan") {
        m_backend = SkiaBackend::VULKAN;
    } else {
        if (platform == "macOS" || platform == "iOS") {
#ifdef __APPLE__
            m_backend = SkiaBackend::METAL;
#else
            WARN_PRINT("We appear to be on macOS or iOS, but __APPLE__ is not defined? Metal support disabled.");
#endif
        }
    }

    if (m_backend == SkiaBackend::UNKNOWN) {
        WARN_PRINT("Unable to detect a platform and renderer-compatible Skia backend. Falling back to CPU default raster renderer.");

        // If all else fails, default to a CPU-based raster driver
        // that _should_ generally be compatible everywhere but is
        // inefficient compared to GPU drawing.
        m_backend = SkiaBackend::RASTER;
    }
}

godot::Error RiveRendererSkia::initialize() {
    // If no backend was manually specified then we attempt to
    // detect a backend best-matched to the current platform.
    if (m_backend == SkiaBackend::UNKNOWN) {
        WARN_PRINT("No Skia backend specified, attempting to automatically detect a best match.");
        _detect_backend();
    }

    return godot::OK;
}

godot::Ref<RiveRenderTarget> RiveRendererSkia::create_render_target(godot::Point2 p_size) {
    godot::Ref<RiveRenderTargetSkia> render_target;
    switch (m_backend) {
        case SkiaBackend::UNKNOWN: {
            ERR_FAIL_V_MSG({}, "No backend selected, this should not happen!");
        } break;
        case SkiaBackend::RASTER: {
            godot::UtilityFunctions::print("Creating new Raster rendering target");
            render_target = godot::Ref<RiveRenderTargetSkia>(memnew(RiveRenderTargetSkiaRaster(p_size)));
        } break;
        case SkiaBackend::OPENGL: {
            godot::UtilityFunctions::print("Creating new OpenGL rendering target");
            render_target = godot::Ref<RiveRenderTargetSkia>(memnew(RiveRenderTargetSkiaOpenGL(p_size)));
        } break;
        case SkiaBackend::METAL: {
#ifdef __APPLE__
            godot::UtilityFunctions::print("Creating new Metal rendering target");
            render_target = godot::Ref<RiveRenderTargetSkia>(memnew(RiveRenderTargetSkiaMetal(p_size)));
#else
            ERR_FAIL_V_MSG({}, "Metal backend is selected, but library was not compiled with __APPLE__ so Metal is unavailable.");
#endif
        } break;
        case SkiaBackend::VULKAN: {
            godot::UtilityFunctions::print("Creating new Vulkan rendering target");
            render_target = godot::Ref<RiveRenderTargetSkia>(memnew(RiveRenderTargetSkiaVulkan(p_size)));
        } break;
    }

    // Initialize the selected render target implementation.
    render_target->_initialize();

    return render_target;
}

rive::Factory *RiveRendererSkia::_get_factory() const {
    static rive::SkiaFactory factory;
    return &factory;
}


