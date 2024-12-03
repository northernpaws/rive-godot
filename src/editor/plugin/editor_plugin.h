

#ifndef RIVE_GODOT_EDITOR_PLUGIN_H
#define RIVE_GODOT_EDITOR_PLUGIN_H

#include <godot_cpp/classes/editor_plugin.hpp>

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/control.hpp>

#include "editor/plugin/screen/editor_main_screen.h"

#include "editor/plugin/inspector/resource/resource_editor_inspector_plugin.h"
#include "editor/plugin/import/editor_import_plugin.h"
#include "editor/plugin/tooltip/editor_resource_tooltip_plugin.h"
#include "editor/plugin/preview/editor_resource_preview_generator.h"
#include "editor/plugin/inspector/drawable/drawable_editor_inspector_plugin.h"

class RiveEditorPlugin : public godot::EditorPlugin {
    GDCLASS(RiveEditorPlugin, godot::EditorPlugin)

    godot::Ref<RiveResourceEditorInspectorPlugin> resource_inspector_plugin;
    godot::Ref<RiveDrawableEditorInspectorPlugin> drawable_inspector_plugin;
    godot::Ref<RiveEditorImportPlugin> import_plugin;
    godot::Ref<RiveEditorResourceTooltipPlugin> resource_tooltip_plugin;
    godot::Ref<RiveEditorResourcePreviewGenerator> resource_preview_generator;

    RiveEditorMainScreen* main_screen_instance = nullptr;
protected:
    static void _bind_methods() {}
public:
    RiveEditorPlugin();
    ~RiveEditorPlugin() override;

    // General plugin metadata.
    [[nodiscard]] godot::String _get_plugin_name() const override;
    // TODO: _get_plugin_icon

    // Method implementations for handling the main screen.
    [[nodiscard]] bool _has_main_screen() const override;

    void _enter_tree() override;
    void _exit_tree() override;

    void _make_visible(bool visible) override;
};


#endif //RIVE_GODOT_EDITOR_PLUGIN_H
