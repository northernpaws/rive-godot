

#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "editor_resource_preview_generator.h"

#include "resources/rive_resource.h"

bool RiveEditorResourcePreviewGenerator::_handles(const godot::String &type) const {
    return type == RiveResource::get_class_static();
}

godot::Ref<godot::Texture2D>
RiveEditorResourcePreviewGenerator::_generate(const godot::Ref<godot::Resource> &resource, const godot::Vector2i &size,
                                              const godot::Dictionary &metadata) const {

    godot::UtilityFunctions::print("[RiveEditorResourcePreviewGenerator] _generate generating preview for ", resource->get_path(), " ", size.width, "x", size.height);

    godot::Ref<RiveResource> rive_resource = resource;
    ERR_FAIL_NULL_V_MSG(rive_resource, {}, "cannot generate preview for non-Rive resource " + rive_resource->get_class());
//    if(file.is_null()) {
//        return {}; // indicates failure and defers to other preview generators
//    }

    // TODO: The GDExtension API is passing constant types for parameters that should be writable
    //  fix source: https://github.com/Zylann/godot_voxel/blob/67245c4b015b82117d4e365c2bebcdc44e7451ad/util/godot/classes/editor_import_plugin.cpp#L142
    auto& metadata_writable = const_cast<godot::Dictionary&>(metadata);

    // Create an image texture for the preview.
    auto image = godot::Image::create(size.width, size.height, false, godot::Image::FORMAT_RGBA8);

    // TODO: remove testing
    image->fill(godot::Color(1.0, 0.0, 0.0));

    // Set metadata for the thumbnail tooltip preview (see RiveEditorResourcePreviewGenerator).
    metadata_writable["default_artboard"] = "todo";
    metadata_writable["default_state_machine"] = "todo";

//    metadata_writable["artboard_count"] = rive_resource->get_artboard_count();

    // Convert to an image texture to upload to the GPU.
    return godot::ImageTexture::create_from_image(image);
}

//godot::Ref<godot::Texture2D>
//RiveEditorResourcePreviewGenerator::_generate_from_path(const godot::String &path, const godot::Vector2i &size,
//                                                        const godot::Dictionary &metadata) const {
//    godot::UtilityFunctions::print("[RiveEditorResourcePreviewGenerator] _generate_from_path generating preview for ", path, " ", size.width, "x", size.height);
//
//    // Returning null texture indicates failure and attempts to defer to other preview generators.
//    //
//    // This method is optional as the editor will default to trying _generate with an imported resource otherwise.
//    return {};
//}

bool RiveEditorResourcePreviewGenerator::_generate_small_preview_automatically() const {
    return true;
}

bool RiveEditorResourcePreviewGenerator::_can_generate_small_preview() const {
    return true;
}
