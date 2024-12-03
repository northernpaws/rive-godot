

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

#include <rive/assets/asset.hpp>
#include <rive/assets/file_asset.hpp>
#include <rive/assets/image_asset.hpp>
#include <rive/assets/font_asset.hpp>
#include <rive/assets/audio_asset.hpp>
#include <rive/file.hpp>
#include <utils/no_op_factory.hpp>

#include "editor_import_plugin.h"

#include "resources/rive_resource.h"

godot::String RiveEditorImportPlugin::_get_importer_name() const {
    return "rive.riv";
}

godot::String RiveEditorImportPlugin::_get_visible_name() const {
    return "Rive Resource";
}

godot::PackedStringArray RiveEditorImportPlugin::_get_recognized_extensions() const {
    godot::PackedStringArray extensions;

    extensions.append("riv");

    return extensions;
}

godot::String RiveEditorImportPlugin::_get_save_extension() const {
    // see: https://docs.godotengine.org/en/stable/classes/class_resourceformatsaver.html#description
    return "tres"; // "rive";
}

godot::String RiveEditorImportPlugin::_get_resource_type() const {
    return RiveResource::get_class_static();
}

int32_t RiveEditorImportPlugin::_get_preset_count() const {
    return 1;
}

godot::String RiveEditorImportPlugin::_get_preset_name(int32_t preset_index) const {
    switch (static_cast<ImportPresets>(preset_index)) {
        case ImportPresets::DEFAULT:
            return "Default";
        default:
            return "Unknown";
    }
}

godot::TypedArray<godot::Dictionary>
RiveEditorImportPlugin::_get_import_options(const godot::String &path, int32_t preset_index) const {
    godot::TypedArray<godot::Dictionary> import_options;

    switch (static_cast<ImportPresets>(preset_index)) {
        case ImportPresets::DEFAULT: {
            /* auto option = godot::Dictionary();
            option["name"] = "option_name";
            option["default_value"] = "default_value"; // property_hint, hint_string, usage
            import_options.append(option); */
        } break;
    }

    return import_options;
}

bool RiveEditorImportPlugin::_get_option_visibility(const godot::String &path, const godot::StringName &option_name,
                                                    const godot::Dictionary &options) const {
    return true;
}

double RiveEditorImportPlugin::_get_priority() const {
    return 1.0f;
}

int32_t RiveEditorImportPlugin::_get_import_order() const {
    return ImportOrder::IMPORT_ORDER_DEFAULT;
}


godot::Error RiveEditorImportPlugin::_import(const godot::String &source_file, const godot::String &save_path,
                                             const godot::Dictionary &options,
                                             const godot::TypedArray<godot::String> &platform_variants,
                                             const godot::TypedArray<godot::String> &gen_files) const {

    // TODO: The GDExtension API is passing constant types for parameters that should be writable
    //  fix source: https://github.com/Zylann/godot_voxel/blob/67245c4b015b82117d4e365c2bebcdc44e7451ad/util/godot/classes/editor_import_plugin.cpp#L142
//    auto& platform_variants_writable = const_cast<godot::TypedArray<godot::String>&>(platform_variants);
//    godot::TypedArray<godot::String>& gen_files_writable = const_cast<godot::TypedArray<godot::String>&>(gen_files);

    godot::Ref<RiveResource> resource = memnew(RiveResource);

    auto bytes = godot::FileAccess::get_file_as_bytes(source_file);
    ERR_FAIL_COND_V_MSG(bytes.is_empty(), godot::FileAccess::get_open_error(), "failed to open .riv file for reading");
    resource->set_bytes(bytes);

    godot::UtilityFunctions::print("importing ", source_file);

    // Import the file using a NoOpFactory as we only need to import to extract assets, we don't actually go on to use the
    // file handle produced by this import any further (that's up to the renderer to derive from the resource later).
    static rive::NoOpFactory factory;
    rive::ImportResult result;
    auto file = rive::File::import(rive::Span(bytes.ptr(), bytes.size()),  &factory, &result);
    ERR_FAIL_COND_V_MSG(result != rive::ImportResult::success, godot::ERR_PARSE_ERROR, "failed to import .riv file");
    // TODO: check rive::ImportResult result;

    if (file->assets().size() > 0) {
        godot::UtilityFunctions::print("processing ", int(file->assets().size()), " embedded assets");
        for (auto asset: file->assets()) {
            godot::UtilityFunctions::print("found riv asset", asset->uniqueFilename().c_str(),
                                           asset->fileExtension().c_str());

            // TODO: actually load assets
            // TODO: use append_import_external_resource so other imports can be called as needed
            //  see: https://rive.app/community/doc/loading-assets/doct4wVHGPgC
            switch (asset->coreType()) {
                case rive::ImageAsset::typeKey: {
//                    auto image_path = godot::vformat("%s.%s.%s", save_path, asset->uniqueFilename().c_str(), _get_save_extension());
//                    auto err = godot::ResourceSaver::get_singleton()->save(resource, image_path);
                } break;
                case rive::FontAsset::typeKey: {

                } break;
                case rive::AudioAsset::typeKey: {

                } break;
                default: {

                } break;
            }
        }

        // Since we've extracted the assets for external import handling via Godot we don't need them saved in the embedded data.
        rive::ImportResult strip_result;
        auto stripped_bytes = rive::File::stripAssets(rive::Span(bytes.ptr(), bytes.size()), {
                rive::ImageAsset::typeKey,
                rive::FontAsset::typeKey,
                rive::AudioAsset::typeKey
            }, &strip_result);
        ERR_FAIL_COND_V_MSG(strip_result != rive::ImportResult::success, godot::FAILED, "failed to strip embedded assets from .riv source");

        // Container conversion from c++ vector to godot vector.
        godot::PackedByteArray stripped_bytes_gd;
        stripped_bytes_gd.resize(stripped_bytes.size());// TODO: is this correct size?
        memcpy(stripped_bytes_gd.ptrw(), stripped_bytes.data(), stripped_bytes.size()); // TODO: is this correct size?

        resource->set_bytes(stripped_bytes_gd);
    }

    // TODO: extract, download, and import any external assets in the rive file and push them to gen_files.append(path)

    // Ensure we save the main file resource BEFORE saving any child resources, otherwise the importer will embed
    // the FULL file resource data (including bytes) in child resources instead of using a path reference.
    auto file_resource_save_path = godot::vformat("%s.%s", save_path, _get_save_extension());
    resource->set_path(file_resource_save_path); // set the path so that derived resources can reference it by path instead of local embedding.
    auto err = godot::ResourceSaver::get_singleton()->save(resource, file_resource_save_path);
    ERR_FAIL_COND_V_MSG(err != godot::OK, err, "Cannot save imported RiveResource to file.");

    return godot::OK;
}




