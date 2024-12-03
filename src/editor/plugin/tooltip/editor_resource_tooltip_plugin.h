

#ifndef RIVE_GODOT_EDITOR_RESOURCE_TOOLTIP_PLUGIN_H
#define RIVE_GODOT_EDITOR_RESOURCE_TOOLTIP_PLUGIN_H

#include <godot_cpp/classes/control.hpp>

#include <godot_cpp/classes/editor_resource_tooltip_plugin.hpp>

class RiveEditorResourceTooltipPlugin : public godot::EditorResourceTooltipPlugin {
    GDCLASS(RiveEditorResourceTooltipPlugin, godot::EditorResourceTooltipPlugin)
protected:
    static void _bind_methods() {}
public:
    [[nodiscard]] bool _handles(const godot::String &type) const override;
    godot::Control* _make_tooltip_for_path(const godot::String &path, const godot::Dictionary &metadata, godot::Control *base) const override;
};


#endif //RIVE_GODOT_EDITOR_RESOURCE_TOOLTIP_PLUGIN_H
