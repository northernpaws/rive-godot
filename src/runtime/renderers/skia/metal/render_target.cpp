

// Needs to be included ONLY in ONE cpp file in the project, if
// included in a header it'll introduce duplicate symbol errors.
#ifndef METAL_INC
#define METAL_INC
// ref: https://developer.apple.com/metal/cpp/
#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#endif

#include <godot_cpp/classes/rendering_server.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include <include/core/SkAlphaType.h>
#include <include/core/SkImage.h>
#include <include/core/SkColorSpace.h>

#include <include/gpu/GrDirectContext.h>
#include <include/gpu/gl/GrGLInterface.h>
#include <include/gpu/mtl/GrMtlBackendContext.h>

#include <skia_renderer.hpp>

#include "render_target.h"

#include "runtime/rive_artboard.h"

void RiveRenderTargetSkiaMetal::_bind_methods() {}

RiveRenderTargetSkiaMetal::RiveRenderTargetSkiaMetal() = default;

RiveRenderTargetSkiaMetal::RiveRenderTargetSkiaMetal(godot::Point2 p_size) : RiveRenderTargetSkia(p_size) {}

RiveRenderTargetSkiaMetal::~RiveRenderTargetSkiaMetal() {}

void RiveRenderTargetSkiaMetal::_create_renderer() {
    ERR_FAIL_COND_MSG(m_size.is_zero_approx(), "Cannot initialize render target with zero size.");

    // Create the backing texture for the render target.
    NS::SharedPtr<MTL::TextureDescriptor> texture_descriptor = NS::TransferPtr( MTL::TextureDescriptor::alloc()->init() );
    // NOTE: Texture format needs to match the format used for the RiveTexture resource we copy to!
    texture_descriptor->setPixelFormat(MTL::PixelFormat::PixelFormatRGBA8Unorm); // PixelFormatEAC_RGBA8);
    texture_descriptor->setWidth(static_cast<NS::UInteger>(get_width()));
    texture_descriptor->setHeight(static_cast<NS::UInteger>(get_height()));
    texture_descriptor->setMipmapLevelCount(1);
    texture_descriptor->setUsage(MTL::TextureUsageRenderTarget);
    m_metal_texture = NS::TransferPtr(m_metal_device->newTexture(texture_descriptor.get()));
    godot::UtilityFunctions::print(godot::vformat("Created Metal texture. width=%d height=%d mipmaps=%d",
                                                  int(m_metal_texture->width()),
                                                  int(m_metal_texture->height()),
                                                  int(m_metal_texture->mipmapLevelCount())));

    m_metal_queue = NS::TransferPtr(m_metal_device->newCommandQueue());

    // Create a Skia backend from the Metal device and command queue.
    auto context = GrMtlBackendContext();
    context.fDevice.retain(m_metal_device.get());
    context.fQueue.retain(m_metal_queue.get());
    m_skia_context = GrDirectContext::MakeMetal(context);

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

    // TODO: resize Mtl texture if needed

    // Create a Skia render target using the texture.
    GrMtlTextureInfo mtl_info;
    mtl_info.fTexture.retain(m_metal_texture.get());
    GrBackendRenderTarget target(static_cast<int>(get_width()), static_cast<int>(get_height()), mtl_info);

    sk_sp<SkColorSpace> colorspace = nullptr; // no colorspace
    SkSurfaceProps      surface_props = SkSurfaceProps(0, kRGB_H_SkPixelGeometry);

    // NOTE: The image type used here is important! It MUST match the layout of the image
    // type used for the Godot image backing the render target in Godot's image space.
    //SkImageInfo info = SkImageInfo::MakeN32Premul(width, height);
//    auto info = SkImageInfo::Make(width, height, SkColorType::kRGBA_8888_SkColorType, SkAlphaType::kPremul_SkAlphaType);
    m_skia_surface = SkSurface::MakeFromBackendRenderTarget(m_skia_context.get(), target,
                                                            GrSurfaceOrigin::kTopLeft_GrSurfaceOrigin,
                                                            SkColorType::kRGBA_8888_SkColorType,
                                                            colorspace,
                                                            &surface_props, nullptr);
    ERR_FAIL_NULL_MSG(m_skia_surface, "Failed to create Skia GPU surface.");

    m_canvas = m_skia_surface->getCanvas();
    m_renderer = std::make_unique<rive::SkiaRenderer>(m_canvas);
}

godot::Error RiveRenderTargetSkiaMetal::_initialize() {
    m_metal_device = NS::TransferPtr(MTL::CreateSystemDefaultDevice());

    _create_renderer();

    return godot::OK;
}

void RiveRenderTargetSkiaMetal::resize(godot::Point2 p_size) {
    ERR_FAIL_COND_MSG(p_size.is_zero_approx(), "Zero size passed to RiveRenderTargetSkiaMetal::resize.");

    if (m_size == p_size) {
        godot::UtilityFunctions::print("RiveRenderTargetSkiaMetal::resize skipped, matches old size");
        return;
    }

    godot::UtilityFunctions::print(godot::vformat("RiveRenderTargetSkiaMetal::resize %dx%d", p_size.width, p_size.height));

    m_size = p_size;

    _create_renderer();
}

void RiveRenderTargetSkiaMetal::_update() {
    // Only applicable to rendering device based targets
//    if (godot::OS::get_singleton()->get_thread_caller_id() != godot::OS::get_singleton()->get_main_thread_id()) {
//        WARN_PRINT("Calling RiveRenderTargetSkiaMetal::_update from a non-main or non-renderserver thread may stall the RenderingServer on texture updates.");
//    }

    // TODO: thread drawing/updating, but ensure texture is only updated from main thread. call_on_render_thread?

    ERR_FAIL_NULL_MSG(m_skia_surface, "Cannot update from a null SkiSurface.");
    ERR_FAIL_COND_MSG(m_skia_surface->width() != static_cast<int>(get_width()), "SkSurface width does not match render target width.");
    ERR_FAIL_COND_MSG(m_skia_surface->height() != static_cast<int>(get_height()), "SkSurface height does not match render target height.");

    // Read the pixels from the GPU-backed surface directly into our frame buffer.
    auto info = SkImageInfo::Make(static_cast<int>(get_width()), static_cast<int>(get_height()), SkColorType::kRGBA_8888_SkColorType, SkAlphaType::kPremul_SkAlphaType);
    auto successful_read = m_skia_surface->readPixels(info, m_frame_data.ptrw(), info.minRowBytes(), 0, 0);
    ERR_FAIL_COND_MSG(!successful_read, "Cannot read pixels from SkSurface!");

    // Create the new image from the frame data and update it in Godot's GPU context.
    godot::Ref<godot::Image> frame_image = godot::Image::create_from_data(
            get_width(), get_height(), false,
            godot::Image::FORMAT_RGBA8, m_frame_data); // TODO: zero copy image creation? https://github.com/godotengine/godot/blob/2a0aef5f0912b60f85c9e150cc0bfbeab7de6e40/modules/theora/video_stream_theora.cpp#LL116C19-L116C25
    m_texture->update(frame_image);
}

void RiveRenderTargetSkiaMetal::_draw_begin(const rive::AABB& content_bounds) {
    // Clear the canvas before drawing.
    //
    // NOTE: We don't call this in the super because the raster target implementation
    // clears its own internal data buffer instead of the canvas directly.
    m_canvas->clear(SkColorSetARGB(255, 0, 0, 0));

    RiveRenderTargetSkia::_draw_begin(content_bounds);
}

void RiveRenderTargetSkiaMetal::_draw_end() {
    RiveRenderTargetSkia::_draw_end();

    // Update the texture data.
    _update();
}

void RiveRenderTargetSkiaMetal::draw_artboard(const godot::Ref<RiveArtboard> &p_artboard) {
    ERR_FAIL_COND_MSG(!p_artboard.is_valid(), "Cannot render null RiveArtboard.");

    // Clear the frame image and set renderer transform and alignment.
    _draw_begin(p_artboard->_get_rive_bounds());

    // Render the object.
    p_artboard->_get_rive_artboard_instance()->draw(m_renderer.get());

    // Update the target texture with the rendered frame.
    _draw_end();
}

void RiveRenderTargetSkiaMetal::draw_animation(const godot::Ref<RiveAnimation> &p_animation) {
    ERR_FAIL_COND_MSG(!p_animation.is_valid(), "Cannot render null RiveAnimation.");

    // Clear the frame image and set renderer transform and alignment.
    _draw_begin(p_animation->_get_rive_bounds());

    // Render the object.
    p_animation->_get_rive_animation_instance()->draw(m_renderer.get());

    // Update the target texture with the rendered frame.
    _draw_end();
}

void RiveRenderTargetSkiaMetal::draw_state_machine(const godot::Ref<RiveStateMachine> &p_state_machine) {
    ERR_FAIL_COND_MSG(!p_state_machine.is_valid(), "Cannot render null RiveStateMachine.");

    // Clear the frame image and set renderer transform and alignment.
    _draw_begin(p_state_machine->_get_rive_bounds());

    // Render the object.
    p_state_machine->_get_rive_state_machine_instance()->draw(m_renderer.get());

    // Update the target texture with the rendered frame.
    _draw_end();
}