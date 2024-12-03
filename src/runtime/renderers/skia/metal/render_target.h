

#ifndef RIVE_GODOT_RENDER_TARGET_METAL_H
#define RIVE_GODOT_RENDER_TARGET_METAL_H

#ifdef SK_METAL

#include <Foundation/NSSharedPtr.hpp>
#include <Metal/MTLDevice.hpp>
#include <Metal/MTLTexture.hpp>
#include <Metal/MTLCommandQueue.hpp>

// Skia
#include <include/core/SkSurface.h>
#include <include/gpu/GrDirectcontext.h>
//#include <include/core/SkCanvas.h>
//#include <include/gpu/mtl/GrMtlTypes.h>
//#include <include/core/SkRefCnt.h>
//#include <include/ports/SkCFObject.h> // CoreFoundation wrapper

#include "runtime/rive_render_target.h"
#include "runtime/renderers/skia/rive_render_target_skia.h"


class RiveRenderTargetSkiaMetal : public RiveRenderTargetSkia {
    GDCLASS(RiveRenderTargetSkiaMetal, RiveRenderTargetSkia)

//    sk_cfp<MTL::Device*> m_metal_device;

    NS::SharedPtr<MTL::Device>  m_metal_device;
    NS::SharedPtr<MTL::Texture> m_metal_texture;
    NS::SharedPtr<MTL::CommandQueue> m_metal_queue;

    godot::PackedByteArray m_frame_data;

    sk_sp<GrDirectContext> m_skia_context;
    sk_sp<SkSurface> m_skia_surface;
protected:
    static void _bind_methods();

    RiveRenderTargetSkiaMetal();

    void _create_renderer();

    void _update();

    void _draw_begin(const rive::AABB& content_bounds) override;
    void _draw_end() override;
public:
    ~RiveRenderTargetSkiaMetal() override;

    explicit RiveRenderTargetSkiaMetal(godot::Point2 p_size);

    godot::Error _initialize() override;

    void resize(godot::Point2 p_size) override;

    void draw_artboard(const godot::Ref<RiveArtboard> &p_artboard) override;
    void draw_animation(const godot::Ref<RiveAnimation> &p_animation) override;
    void draw_state_machine(const godot::Ref<RiveStateMachine> &p_state_machine) override;


};

#endif

#endif //RIVE_GODOT_RENDER_TARGET_H
