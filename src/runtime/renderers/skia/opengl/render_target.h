

#ifndef RIVE_GODOT_RENDER_TARGET_OPENGL_H
#define RIVE_GODOT_RENDER_TARGET_OPENGL_H

// Ignore deprecation errors emitted by macOS.
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include <rive/renderer.hpp>

#include <include/core/SkSurface.h>
#include <include/core/SkCanvas.h>

#include <include/gpu/GrDirectContext.h>
#include <include/gpu/gl/GrGLInterface.h>

#include "runtime/rive_render_target.h"
#include "runtime/renderers/skia/rive_render_target_skia.h"

class RiveRenderTargetSkiaOpenGL : public RiveRenderTargetSkia {
    GDCLASS(RiveRenderTargetSkiaOpenGL, RiveRenderTargetSkia)

    // the last rendered frame
//    godot::PackedByteArray m_frame_data;

    /// in-memory image to be updated frame-by-frame.
    // TODO: circular image buffer?
    // TODO: write directly to texture image?
    godot::PackedByteArray m_frame_data;
//    godot::Ref<godot::Image> m_image;

    GLFWwindow* m_window = nullptr;

    SkImageInfo m_info;
    sk_sp<const GrGLInterface> m_interface = nullptr;
    sk_sp<GrDirectContext> m_context;

    sk_sp<SkSurface> m_surface;
protected:
    static void _bind_methods();

    RiveRenderTargetSkiaOpenGL();

    void _create_renderer();

    void _update();

    void _draw_begin(const rive::AABB& content_bounds) override;
    void _draw_end() override;
public:
    ~RiveRenderTargetSkiaOpenGL() override;

    explicit RiveRenderTargetSkiaOpenGL(godot::Point2 p_size);

    godot::Error _initialize() override;

    void resize(godot::Point2 p_size) override;

    void draw_artboard(const godot::Ref<RiveArtboard> &p_artboard) override;
    void draw_animation(const godot::Ref<RiveAnimation> &p_animation) override;
    void draw_state_machine(const godot::Ref<RiveStateMachine> &p_state_machine) override;
};


#endif //RIVE_GODOT_RENDER_TARGET_H
