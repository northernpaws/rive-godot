

#include "drawable_editor_inspector_plugin.h"

#include "resources/rive_drawable.h"
#include "rive_drawable_property_editor_control.h"

bool RiveDrawableEditorInspectorPlugin::_can_handle(godot::Object *object) const {
    // Handles all types so we can draw the custom fields for any RiveDrawable properties.
    return true;
}


void RiveDrawableEditorInspectorPlugin::_parse_begin(godot::Object *object) {
    godot::Ref<RiveDrawable> resource(godot::Object::cast_to<RiveDrawable>(object));
    ERR_FAIL_NULL_MSG(resource, "null RiveDrawable resource passed to inspector plugin.");


}

bool RiveDrawableEditorInspectorPlugin::_parse_property(godot::Object *object, godot::Variant::Type type,
                                                        const godot::String &name, godot::PropertyHint hint_type,
                                                        const godot::String &hint_string,
                                                        godot::BitField<godot::PropertyUsageFlags> usage_flags,
                                                        bool wide) {

    if(object->get_class() == RiveDrawable::get_class_static() || (hint_type == godot::PropertyHint::PROPERTY_HINT_RESOURCE_TYPE && hint_string == RiveDrawable::get_class_static())) {
        add_property_editor(name, memnew(RiveDrawablePropertyEditorControl));
        return true;
    }

    return false;
}

void RiveDrawableEditorInspectorPlugin::_parse_end(godot::Object *object) {

}


