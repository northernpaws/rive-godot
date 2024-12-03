

#ifndef RIVE_GODOT_RESOURCE_EDITOR_INSPECTOR_PLUGIN_H
#define RIVE_GODOT_RESOURCE_EDITOR_INSPECTOR_PLUGIN_H

#include <godot_cpp/classes/editor_inspector_plugin.hpp>

class RiveResourceEditorInspectorPlugin : public godot::EditorInspectorPlugin {
    GDCLASS(RiveResourceEditorInspectorPlugin, godot::EditorInspectorPlugin)
protected:
    static void _bind_methods() {}
public:

    bool _can_handle(godot::Object *object) const override;

    void _parse_begin(godot::Object *object) override;
    void _parse_end(godot::Object *object) override;
};


#endif //RIVE_GODOT_RESOURCE_EDITOR_INSPECTOR_PLUGIN_H
