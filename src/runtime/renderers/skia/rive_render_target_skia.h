

#ifndef RIVE_GODOT_RIVE_RENDER_TARGET_SKIA_H
#define RIVE_GODOT_RIVE_RENDER_TARGET_SKIA_H

#include <rive/renderer.hpp>

#include <include/core/SkSurface.h>
#include <include/core/SkCanvas.h>

#include "runtime/rive_render_target.h"

/**
 * Abstract base for Skia backend implementations.
 */
class RiveRenderTargetSkia : public RiveRenderTarget {
    GDCLASS(RiveRenderTargetSkia, RiveRenderTarget)
protected:
    SkCanvas* m_canvas;
    std::unique_ptr<rive::Renderer> m_renderer;

    static void _bind_methods();

    RiveRenderTargetSkia() = default;

    explicit RiveRenderTargetSkia(godot::Point2 p_size);

    virtual void _draw_begin(const rive::AABB& content_bounds);
    virtual void _draw_end();
public:
};


#endif //RIVE_GODOT_RIVE_RENDER_TARGET_SKIA_H
