

#ifndef RIVE_GODOT_EDITOR_RESOURCE_PICKER_RIVE_RESOURCE_H
#define RIVE_GODOT_EDITOR_RESOURCE_PICKER_RIVE_RESOURCE_H

#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/editor_resource_picker.hpp>

class EditorResourcePickerRiveResource : public godot::EditorResourcePicker {
    GDCLASS(EditorResourcePickerRiveResource, godot::EditorResourcePicker)
protected:
    static void _bind_methods() {}
public:

    // Override to hide the default "New..." option.
    void _set_create_options(godot::Object *menu_node) override;
};


#endif //RIVE_GODOT_EDITOR_RESOURCE_PICKER_RIVE_RESOURCE_H
