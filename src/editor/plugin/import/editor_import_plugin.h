

#ifndef RIVE_GODOT_EDITOR_IMPORT_PLUGIN_H
#define RIVE_GODOT_EDITOR_IMPORT_PLUGIN_H

#include <godot_cpp/classes/editor_import_plugin.hpp>

class RiveEditorImportPlugin : public godot::EditorImportPlugin {
    GDCLASS(RiveEditorImportPlugin, godot::EditorImportPlugin)
protected:
    static void _bind_methods() {}
public:
    enum class ImportPresets: int32_t {
        DEFAULT = 0
    };

    [[nodiscard]] godot::String _get_importer_name() const override;
    [[nodiscard]] godot::String _get_visible_name() const override;

    [[nodiscard]] godot::PackedStringArray _get_recognized_extensions() const override;

    [[nodiscard]] godot::String _get_save_extension() const override;

    [[nodiscard]] godot::String _get_resource_type() const override;

    [[nodiscard]] int32_t _get_preset_count() const override;
    [[nodiscard]] godot::String _get_preset_name(int32_t preset_index) const override;
    [[nodiscard]] godot::TypedArray<godot::Dictionary> _get_import_options(const godot::String &path, int32_t preset_index) const override;
    [[nodiscard]] bool _get_option_visibility(const godot::String &path, const godot::StringName &option_name, const godot::Dictionary &options) const override;

    [[nodiscard]] double _get_priority() const override;
    [[nodiscard]] int32_t _get_import_order() const override;

    [[nodiscard]] godot::Error _import(const godot::String &source_file, const godot::String &save_path, const godot::Dictionary &options, const godot::TypedArray<godot::String> &platform_variants, const godot::TypedArray<godot::String> &gen_files) const override;
};


#endif //RIVE_GODOT_EDITOR_IMPORT_PLUGIN_H
