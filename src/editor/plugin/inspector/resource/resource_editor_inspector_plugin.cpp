

#include "resource_editor_inspector_plugin.h"

#include "resource_inspector_preview_control.h"
#include "resources/rive_resource.h"

bool RiveResourceEditorInspectorPlugin::_can_handle(godot::Object *object) const {
    return object->is_class(RiveResource::get_class_static());
}


void RiveResourceEditorInspectorPlugin::_parse_begin(godot::Object *object) {
    godot::Ref<RiveResource> resource(godot::Object::cast_to<RiveResource>(object));
    ERR_FAIL_NULL_MSG(resource, "null Rive resource passed to inspector plugin.");

    // Add the file preview to the top of the inspector.
    add_custom_control(memnew(RiveResourceInspectorPreviewControl(resource)));
}

void RiveResourceEditorInspectorPlugin::_parse_end(godot::Object *object) {

}

