

#ifndef RIVE_GODOT_RIVE_RENDERER_SKIA_H
#define RIVE_GODOT_RIVE_RENDERER_SKIA_H

#include <godot_cpp/classes/image_texture.hpp>

#include <rive/pls/pls_render_context.hpp>

#include "runtime/rive_renderer.h"

/**
 * Rive renderer implementation that uses Rive's PLS renderer.
 */
class RiveRendererPLS : public RiveRenderer {
    GDCLASS(RiveRendererPLS, RiveRenderer)
public:
    enum class PLSBackend {
        UNKNOWN,
        OPENGL
        // TODO: metal, vulkan, webgpu (, d3d?)
    };
private:
    PLSBackend m_backend = PLSBackend::UNKNOWN;

    std::unique_ptr<rive::pls::PLSRenderContext> m_pls_render_context = nullptr;
protected:
    static void _bind_methods();
public:
    godot::Error initialize() override;
    godot::Ref<RiveRenderTarget> create_render_target(godot::Point2 p_size) override;
    rive::Factory* _get_factory() const override;
};


#endif //RIVE_GODOT_RIVE_RENDERER_SKIA_H
