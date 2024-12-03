



#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/image_texture.hpp>

#include <skia_renderer.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "render_target.h"

#include "runtime/rive_artboard.h"

void RiveRenderTargetSkiaRaster::_bind_methods() {

}

RiveRenderTargetSkiaRaster::RiveRenderTargetSkiaRaster() = default;

RiveRenderTargetSkiaRaster::RiveRenderTargetSkiaRaster(godot::Point2 p_size) : RiveRenderTargetSkia(p_size) {
}


void RiveRenderTargetSkiaRaster::_create_renderer() {
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

    // Create a new surface for the new size.
    godot::UtilityFunctions::print(godot::vformat("creating SkSurface for %dx%d texture rid=%d", m_size.width, m_size.height, m_texture->get_rid().get_id()));
    auto info = SkImageInfo::Make(width, height, SkColorType::kRGBA_8888_SkColorType, SkAlphaType::kPremul_SkAlphaType);
    m_surface = SkSurface::MakeRasterDirect(info, m_frame_data.ptrw(), info.minRowBytes(), nullptr);
    ERR_FAIL_NULL_MSG(m_surface, "Failed to create Skia raster surface.");

    // Get the derived canvas and create a renderer from it.
    m_canvas = m_surface->getCanvas();
    m_renderer = std::make_unique<rive::SkiaRenderer>(m_canvas);
}


godot::Error RiveRenderTargetSkiaRaster::_initialize() {
    _create_renderer();

    return godot::OK;
}

void RiveRenderTargetSkiaRaster::resize(godot::Point2 p_size) {
    ERR_FAIL_COND_MSG(p_size.is_zero_approx(), "Zero size passed to RiveRenderTargetSkiaRaster::resize.");

    if (m_size == p_size) {
        godot::UtilityFunctions::print("RiveRenderTargetSkiaRaster::resize skipped, matches old size");
        return;
    }

    godot::UtilityFunctions::print(godot::vformat("RiveRenderTargetSkiaRaster::resize %dx%d", p_size.width, p_size.height));

    m_size = p_size;

    // For this CPU raster backend, we re-create the SkSurface, SkCanvas, and Rive renderer every time the target size
    // changes. This is because with Raster rendering we don't have any GPU-backed Skia resources we can dynamically
    // resize, so we just need to recreate the entire surface.
    _create_renderer();
}

void RiveRenderTargetSkiaRaster::_update() {
    // Only applicable to rendering device based targets
//    if (godot::OS::get_singleton()->get_thread_caller_id() != godot::OS::get_singleton()->get_main_thread_id()) {
//        WARN_PRINT("Calling RiveRenderTargetSkiaRaster::_update from a non-main or non-renderserver thread may stall the RenderingServer on texture updates.");
//    }

    // TODO: thread drawing/updating, but ensure texture is only updated from main thread. call_on_render_thread?

    godot::Ref<godot::Image> frame_image = godot::Image::create_from_data(
            get_width(), get_height(), false,
            godot::Image::FORMAT_RGBA8, m_frame_data); // TODO: zero copy image creation? https://github.com/godotengine/godot/blob/2a0aef5f0912b60f85c9e150cc0bfbeab7de6e40/modules/theora/video_stream_theora.cpp#LL116C19-L116C25
    m_texture->update(frame_image);

    // Update the data of the texture on the GPU with the new image data.
    /*if (m_texture->get_size() == m_image->get_size()) {
        godot::UtilityFunctions::print(godot::vformat("updating GPU texture %dx%d %d with skia rendered frame",
                                                      m_texture->get_width(),  m_texture->get_height(),
                                                      m_texture->get_rid().get_id()));

        m_image->set_data(m_image->get_width(), m_image->get_height(), false, m_image->get_format(), m_frame_data);

        // testing
        godot::Ref<godot::Image> frameImage = godot::Image::create_from_data(m_image->get_width(), m_image->get_height(), false, godot::Image::FORMAT_RGBA8, m_frame_data); // TODO: zero copy image creation? https://github.com/godotengine/godot/blob/2a0aef5f0912b60f85c9e150cc0bfbeab7de6e40/modules/theora/video_stream_theora.cpp#LL116C19-L116C25

        ERR_FAIL_COND_MSG(frameImage.is_null(), "Invalid image");
        ERR_FAIL_COND_MSG(m_texture.is_null(), "Texture is not initialized.");
        ERR_FAIL_COND_MSG(m_texture->get_width() != m_image->get_width() || m_texture->get_height() != m_image->get_height(),
                          "The new image dimensions must match the texture size.");
//        ERR_FAIL_COND_MSG(p_image->get_format() != format,
//                          "The new image format must match the texture's image format.");
//        ERR_FAIL_COND_MSG(mipmaps != p_image->has_mipmaps(),
//                          "The new image mipmaps configuration must match the texture's image mipmaps configuration");

        godot::RenderingServer::get_singleton()->texture_2d_update(m_texture->get_rid(), frameImage, 0);
    } else {
//        auto new_texture = godot::RenderingServer::get_singleton()->texture_2d_create(m_image);
//        godot::RenderingServer::get_singleton()->texture_replace(m_texture->get_rid(), new_texture);
        godot::UtilityFunctions::print("Reinitializing Skia target image to match new texture size.");
        _initialize(); // reinitialize the image with the new texture size.
    }*/
}

void RiveRenderTargetSkiaRaster::_draw_begin(const rive::AABB& content_bounds) {
    // Perform a "clear" on the image data.
    //
    // NOTE: clearing the canvas pixel buffer with memset is faster than using clear/drawColor.
    memset(m_frame_data.ptrw(), 0, m_frame_data.size());

    RiveRenderTargetSkia::_draw_begin(content_bounds);
}

void RiveRenderTargetSkiaRaster::_draw_end() {
    RiveRenderTargetSkia::_draw_end();

    // Update the texture data.
    _update();
}

void RiveRenderTargetSkiaRaster::draw_artboard(const godot::Ref<RiveArtboard> &p_artboard) {
    ERR_FAIL_COND_MSG(!p_artboard.is_valid(), "Cannot render null RiveArtboard.");

    // Clear the frame image and set renderer transform and alignment.
    _draw_begin(p_artboard->_get_rive_bounds());

    // Render the object.
    p_artboard->_get_rive_artboard_instance()->draw(m_renderer.get());

    // Update the target texture with the rendered frame.
    _draw_end();
}

void RiveRenderTargetSkiaRaster::draw_animation(const godot::Ref<RiveAnimation> &p_animation) {
    ERR_FAIL_COND_MSG(!p_animation.is_valid(), "Cannot render null RiveAnimation.");

    // Clear the frame image and set renderer transform and alignment.
    _draw_begin(p_animation->_get_rive_bounds());

    // Render the object.
    p_animation->_get_rive_animation_instance()->draw(m_renderer.get());

    // Update the target texture with the rendered frame.
    _draw_end();
}

void RiveRenderTargetSkiaRaster::draw_state_machine(const godot::Ref<RiveStateMachine> &p_state_machine) {
    ERR_FAIL_COND_MSG(!p_state_machine.is_valid(), "Cannot render null RiveStateMachine.");

    // Clear the frame image and set renderer transform and alignment.
    _draw_begin(p_state_machine->_get_rive_bounds());

    // Render the object.
    p_state_machine->_get_rive_state_machine_instance()->draw(m_renderer.get());

    // Update the target texture with the rendered frame.
    _draw_end();
}