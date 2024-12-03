



#include <godot_cpp/classes/rendering_server.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include <include/core/SkAlphaType.h>
#include <include/gpu/gl/egl/GrGLMakeEGLInterface.h>

#include <skia_renderer.hpp>

#include "render_target.h"

#include "runtime/rive_artboard.h"

void RiveRenderTargetSkiaOpenGL::_bind_methods() {

}

RiveRenderTargetSkiaOpenGL::RiveRenderTargetSkiaOpenGL() = default;

RiveRenderTargetSkiaOpenGL::RiveRenderTargetSkiaOpenGL(godot::Point2 p_size) : RiveRenderTargetSkia(p_size) {
}

RiveRenderTargetSkiaOpenGL::~RiveRenderTargetSkiaOpenGL() {
    if(m_window != nullptr) {
        glfwDestroyWindow(m_window);
    }
}


void RiveRenderTargetSkiaOpenGL::_create_renderer() {
    ERR_FAIL_COND_MSG(m_size.is_zero_approx(), "Cannot initialize render target with zero size.");

    // Rive works in floats for sizes, so we need to round to whole numbers for GPU image sizing.
    auto width = static_cast<int>(get_width());
    auto height = static_cast<int>(get_height());

    // (Re)initialize the texture image.
    auto base_image = godot::Image::create(width, height, false, godot::Image::Format::FORMAT_RGBA8);
    base_image->fill(godot::Color(0,0,0)); // fill with black so that we can see the control when there is nothing rendered.
    m_texture->set_image(base_image);

    // Resize the frame data buffer to fit the image.
    m_frame_data.resize(width * height * 4);

    // Wipe the frame data buffer.
    memset(m_frame_data.ptrw(), 0, m_frame_data.size());

    // NOTE: The image type used here is important! It MUST match the layout of the image
    //  type used for the Godot image backing the render target in Godot's image space.
    m_info = SkImageInfo::Make(width, height, SkColorType::kRGBA_8888_SkColorType, SkAlphaType::kPremul_SkAlphaType);
    m_surface = SkSurface::MakeRenderTarget(m_context.get(), SkBudgeted::kNo, m_info);
    ERR_FAIL_NULL_MSG(m_surface, "Failed to create Skia GPU surface.");

    m_canvas = m_surface->getCanvas();

    m_renderer = std::make_unique<rive::SkiaRenderer>(m_canvas);
}

void glfw_opengl_error_callback(int code, const char* description) {
    ERR_PRINT(godot::vformat("A GLFW error occurred code=%s: %s", code, description));
}

godot::Error RiveRenderTargetSkiaOpenGL::_initialize() {
#ifdef __APPLE__
    // Hides the main menu and dock icon on macOS for our headless window(s).
    glfwInitHint(GLFW_COCOA_MENUBAR, GLFW_FALSE);
#endif
    if (!glfwInit()) {
        ERR_PRINT("Failed to initialize GLFW library for the Rive Renderer's OpenGL-based Skia backend.");
        return godot::FAILED;
    }

    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    godot::UtilityFunctions::print(godot::vformat("Using GLFW %d.%d.%d", major, minor, revision));
    godot::UtilityFunctions::print(glfwGetVersionString());

    glfwSetErrorCallback(glfw_opengl_error_callback);

    // Set the minimum required OpenGL version.
    //
    // NOTES:
    //  macOS supports at most 4.1
    //
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // NOTE: Some other window hints, such as transparency, introduce SIGNIFICANT
    //  lag into the rendering and window resizing processes.

    // Create an invisible window for offscreen rendering.
    godot::UtilityFunctions::print("Creating off-screen GLFW context for Rive's Skia renderer");
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    m_window = glfwCreateWindow(
            static_cast<int>(get_width()),
            static_cast<int>(get_height()),
            "Rive OpenGL Renderer",
            NULL, NULL);
    ERR_FAIL_NULL_V_MSG(m_window, godot::FAILED, "Failed to create offscreen GLFW window for Skia.");

    glfwMakeContextCurrent(m_window);

    auto gl_Version = glGetString(GL_VERSION);
    godot::UtilityFunctions::print(godot::vformat("Using OpenGL version %d", gl_Version));

    // TODO: Use of GrGLMakeNativeInterface() SHOULD support iOS, Mac, Android, WebGL, etc. but will need to be tested.
    godot::UtilityFunctions::print("Creating Skia rendering context");
    auto interface = GrGLMakeNativeInterface();
    m_context = GrDirectContext::MakeGL(interface);

    _create_renderer();

    return godot::OK;
}

void RiveRenderTargetSkiaOpenGL::resize(godot::Point2 p_size) {
    ERR_FAIL_COND_MSG(p_size.is_zero_approx(), "Zero size passed to RiveRenderTargetSkiaOpenGL::resize.");

    if (m_size == p_size) {
        godot::UtilityFunctions::print("RiveRenderTargetSkiaOpenGL::resize skipped, matches old size");
        return;
    }

    godot::UtilityFunctions::print(godot::vformat("RiveRenderTargetSkiaOpenGL::resize %dx%d", p_size.width, p_size.height));

    m_size = p_size;

    // For this CPU raster backend, we re-create the SkSurface, SkCanvas, and Rive renderer every time the target size
    // changes. This is because with Raster rendering we don't have any GPU-backed Skia resources we can dynamically
    // resize, so we just need to recreate the entire surface.
    _create_renderer();
}

void RiveRenderTargetSkiaOpenGL::_update() {
    // Only applicable to rendering device based targets
//    if (godot::OS::get_singleton()->get_thread_caller_id() != godot::OS::get_singleton()->get_main_thread_id()) {
//        WARN_PRINT("Calling RiveRenderTargetSkiaOpenGL::_update from a non-main or non-renderserver thread may stall the RenderingServer on texture updates.");
//    }

    // TODO: thread drawing/updating, but ensure texture is only updated from main thread. call_on_render_thread?

    ERR_FAIL_NULL_MSG(m_surface, "Cannot update from a null SkiSurface.");
    ERR_FAIL_COND_MSG(m_surface->width() != static_cast<int>(get_width()), "SkSurface width does not match render target width.");
    ERR_FAIL_COND_MSG(m_surface->height() != static_cast<int>(get_height()), "SkSurface height does not match render target height.");

    // Read the pixels from the GPU-backed surface directly into our frame buffer.
    auto successful_read = m_surface->readPixels(m_info, m_frame_data.ptrw(), m_info.minRowBytes(), 0, 0);
    ERR_FAIL_COND_MSG(!successful_read, "Cannot read pixels from SkSurface!");

    // Create the new image from the frame data and update it in Godot's GPU context.
    godot::Ref<godot::Image> frame_image = godot::Image::create_from_data(
            get_width(), get_height(), false,
            godot::Image::FORMAT_RGBA8, m_frame_data); // TODO: zero copy image creation? https://github.com/godotengine/godot/blob/2a0aef5f0912b60f85c9e150cc0bfbeab7de6e40/modules/theora/video_stream_theora.cpp#LL116C19-L116C25
    m_texture->update(frame_image);
}

void RiveRenderTargetSkiaOpenGL::_draw_begin(const rive::AABB& content_bounds) {
    glfwMakeContextCurrent(m_window);

    // TODO: do we need to clear or does skia handle that?
    //glClearColor(0.8, 0.6, 0.7, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Clear the canvas before drawing.
    //
    // NOTE: We don't call this in the super because the raster target implementation
    // clears its own internal data buffer instead of the canvas directly.
    m_canvas->clear(SkColorSetARGB(0, 0, 0, 0));

    RiveRenderTargetSkia::_draw_begin(content_bounds);
}

void RiveRenderTargetSkiaOpenGL::_draw_end() {
    RiveRenderTargetSkia::_draw_end();

    // TODO: need to swap buffers?
    // eglSwapBuffers(egl_display, egl_surface);

    // Update the texture data.
    _update();
}

void RiveRenderTargetSkiaOpenGL::draw_artboard(const godot::Ref<RiveArtboard> &p_artboard) {
    ERR_FAIL_COND_MSG(!p_artboard.is_valid(), "Cannot render null RiveArtboard.");

    // Clear the frame image and set renderer transform and alignment.
    _draw_begin(p_artboard->_get_rive_bounds());

    // Render the object.
    p_artboard->_get_rive_artboard_instance()->draw(m_renderer.get());

    // Update the target texture with the rendered frame.
    _draw_end();
}

void RiveRenderTargetSkiaOpenGL::draw_animation(const godot::Ref<RiveAnimation> &p_animation) {
    ERR_FAIL_COND_MSG(!p_animation.is_valid(), "Cannot render null RiveAnimation.");

    // Clear the frame image and set renderer transform and alignment.
    _draw_begin(p_animation->_get_rive_bounds());

    // Render the object.
    p_animation->_get_rive_animation_instance()->draw(m_renderer.get());

    // Update the target texture with the rendered frame.
    _draw_end();
}

void RiveRenderTargetSkiaOpenGL::draw_state_machine(const godot::Ref<RiveStateMachine> &p_state_machine) {
    ERR_FAIL_COND_MSG(!p_state_machine.is_valid(), "Cannot render null RiveStateMachine.");

    // Clear the frame image and set renderer transform and alignment.
    _draw_begin(p_state_machine->_get_rive_bounds());

    // Render the object.
    p_state_machine->_get_rive_state_machine_instance()->draw(m_renderer.get());

    // Update the target texture with the rendered frame.
    _draw_end();
}