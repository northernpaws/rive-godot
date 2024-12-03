

#ifndef RIVE_GODOT_RIVE_DRAWABLE_PROPERTY_EDITOR_CONTROL_H
#define RIVE_GODOT_RIVE_DRAWABLE_PROPERTY_EDITOR_CONTROL_H

#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/editor_inspector.hpp>

#include "resources/rive_drawable.h"
#include "editor_resource_picker_rive_resource.h"
#include "rive_drawable_picker_dialog.h"
#include "resources/rive_resource.h"

class RiveDrawablePropertyEditorControl : public godot::EditorProperty {
    GDCLASS(RiveDrawablePropertyEditorControl, godot::EditorProperty)

    godot::Ref<RiveDrawable> m_current_value;

//    godot::Button* m_property_button = nullptr;

    EditorResourcePickerRiveResource *m_drawable_picker = nullptr;
//
//    godot::EditorInspector *sub_inspector = nullptr;
//    bool opened_editor = false;
//
    RiveDrawablePickerDialog* m_dialog;

    void _resource_selected(const godot::Ref<RiveResource> &p_resource, bool p_inspect);
    void _resource_changed(const godot::Ref<RiveResource> &p_resource);
protected:
    static void _bind_methods();

    void _on_button_pressed();
public:
    RiveDrawablePropertyEditorControl();

    void _update_property() override;
    void _set_read_only(bool read_only) override;
};


#endif //RIVE_GODOT_RIVE_DRAWABLE_PROPERTY_EDITOR_CONTROL_H
