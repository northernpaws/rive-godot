

#ifndef RIVE_GODOT_INSPECTOR_PREVIEW_CONTROL_H
#define RIVE_GODOT_INSPECTOR_PREVIEW_CONTROL_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/margin_container.hpp>

#include "resources/rive_resource.h"

class RiveResourceInspectorPreviewControl : public godot::MarginContainer {
    GDCLASS(RiveResourceInspectorPreviewControl, godot::MarginContainer)

    RiveResource m_resource;
protected:
    static void _bind_methods() {}
public:
    RiveResourceInspectorPreviewControl();
    RiveResourceInspectorPreviewControl(godot::Ref<RiveResource> p_resource);
    ~RiveResourceInspectorPreviewControl();
};


#endif //RIVE_GODOT_INSPECTOR_PREVIEW_CONTROL_H
