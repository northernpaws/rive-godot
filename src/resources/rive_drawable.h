

#ifndef RIVE_GODOT_RIVE_DRAWABLE_H
#define RIVE_GODOT_RIVE_DRAWABLE_H

#include <godot_cpp/classes/resource.hpp>

#include "rive_resource.h"

/**
 * A proxy helper for referencing and calling various drawable types in a Rive file.
 *
 * Also provides convenience property editors using an inspector plugin.
 */
class RiveDrawable : public godot::Resource {
    GDCLASS(RiveDrawable, godot::Resource)

    godot::Ref<RiveResource> m_resource;
protected:
    static void _bind_methods();
public:
};


#endif //RIVE_GODOT_RIVE_DRAWABLE_H
