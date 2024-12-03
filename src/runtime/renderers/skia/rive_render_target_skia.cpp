

#include <godot_cpp/variant/utility_functions.hpp>

#include "rive_render_target_skia.h"
#include "runtime/rive_artboard.h"

void RiveRenderTargetSkia::_bind_methods() {

}

RiveRenderTargetSkia::RiveRenderTargetSkia(godot::Point2 p_size) : RiveRenderTarget(p_size) {
}

void RiveRenderTargetSkia::_draw_begin(const rive::AABB &content_bounds) {
    // NOTE: We don't clear the canvas in this super because some implementation (i.e. CPU-based raster)
    // clears its own internal VRAM-backed data buffer instead of clearing the Skia canvas directly.

    // Save the previous state of the renderer.
    m_renderer->save();

    auto view_transform = rive::computeAlignment(rive::Fit::contain,
                                                 rive::Alignment::center,
                                                 rive::AABB(0, 0, get_width(), get_height()),
                                                 content_bounds);

    // Set the transform for the renderer.
    //
    // This handles fitting the rendered graphic into
    // the view box defined by get_width and get_height.
    m_renderer->transform(view_transform);
}

void RiveRenderTargetSkia::_draw_end() {
    // Restore the previous state of the renderer.
    m_renderer->restore();

    // TODO: is flush needed for the raster backend?
    //  see: https://github.com/google/skia/blob/42d1e8f051c7505d79bff79ead7fe1b136294c7a/RELEASE_NOTES.md?plain=1#L368
    // Flush any pending commands to the canvas before reading the pixel data.
    m_canvas->flush();
}
