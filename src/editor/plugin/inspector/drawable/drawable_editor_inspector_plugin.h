

#ifndef RIVE_GODOT_DRAWABLE_EDITOR_INSPECTOR_PLUGIN_H
#define RIVE_GODOT_DRAWABLE_EDITOR_INSPECTOR_PLUGIN_H

#include <godot_cpp/classes/editor_inspector_plugin.hpp>

#include "resources/rive_drawable.h"

class RiveDrawableEditorInspectorPlugin : public godot::EditorInspectorPlugin {
    GDCLASS(RiveDrawableEditorInspectorPlugin, godot::EditorInspectorPlugin)


protected:
    static void _bind_methods() {}
public:

    bool _can_handle(godot::Object *object) const override;
    void _parse_begin(godot::Object *object) override;
    bool _parse_property(godot::Object *object, godot::Variant::Type type, const godot::String &name, godot::PropertyHint hint_type, const godot::String &hint_string, godot::BitField<godot::PropertyUsageFlags> usage_flags, bool wide) override;
    void _parse_end(godot::Object *object) override;
};

#endif //RIVE_GODOT_DRAWABLE_EDITOR_INSPECTOR_PLUGIN_H
