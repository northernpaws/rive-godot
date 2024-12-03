

#ifndef RIVE_GODOT_RIVE_RENDERER_SKIA_H
#define RIVE_GODOT_RIVE_RENDERER_SKIA_H

#include <godot_cpp/classes/image_texture.hpp>

#include <rive/renderer.hpp>

#include <include/core/SkSurface.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkBitmap.h>

#include "runtime/rive_renderer.h"

/**
 * Rive renderer implementation that uses Skia for rendering.
 *
 * Relatively inefficient as every frame needs to be copied between the VRAM and GPU:
 * - raster: VRAM -> GPU (godot::RenderingServer)
 * - opengl: GPU (ogl context) -> VRAM -> GPU (godot::RenderingServer)
 * - vulkan: GPU (vk context) -> VRAM -> GPU (godot::RenderingServer)
 *
 * The rendering targets attempt to do this as efficiently as possible by using common
 * image and texture formats so that no format decoding or conversion is necessary, just
 * a raw data copy.
 *
 * The backend used by Skia (CPU raster rendering, OpenGL, or Vulkan) is based on best
 * compatability with the target platform and the rendering method and rendering driver
 * in use by the Godot engine (i.e. Forward+, Mobile, and Compatability).
 *
 * In general, the Skia renderer will attempt to use a rendering backend that matches
 * the backend Godot is using as closely as possible to ensure compatability with the
 * devices targeted by the export platform.
 */
class RiveRendererSkia : public RiveRenderer {
    GDCLASS(RiveRendererSkia, RiveRenderer)

public:
    enum class SkiaBackend {
        UNKNOWN = 0,
        RASTER = 1,
        OPENGL = 2,
#ifdef __APPLE__
        METAL = 3,
#endif
        VULKAN = 4
    };

private:
    // TODO: expand to more backends then raster

    SkiaBackend m_backend = SkiaBackend::UNKNOWN;

    godot::Ref<godot::ImageTexture> m_texture;
    godot::PackedByteArray m_frame_data; // TODO: write directly to image instead of using framedata in between

    sk_sp<SkSurface> surface;
    SkCanvas* canvas;
    std::unique_ptr<rive::Renderer> renderer;

    void _detect_backend();
protected:
    static void _bind_methods();
public:
    RiveRendererSkia() = default;
    RiveRendererSkia(SkiaBackend p_backend) : m_backend(p_backend) {};

    godot::Error initialize() override;

    godot::Ref<RiveRenderTarget> create_render_target(godot::Point2 p_size) override;

    rive::Factory* _get_factory() const override;
};


#endif //RIVE_GODOT_RIVE_RENDERER_SKIA_H
