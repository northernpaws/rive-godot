

#ifndef RIVE_GODOT_RIVE_CONTROL_H
#define RIVE_GODOT_RIVE_CONTROL_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/image_texture.hpp>

#include "resources/rive_resource.h"
#include "runtime/rive_file.h"
#include "runtime/rive_render_target.h"

class RiveControl : public godot::Control {
    GDCLASS(RiveControl, godot::Control)

    godot::Ref<RiveResource> m_resource;
    godot::Ref<RiveFile> m_file; // derived from m_resource
    godot::Ref<RiveArtboard> m_artboard; // derived from m_file

    godot::Ref<RiveRenderTarget> m_render_target;

    void _resource_changed();
    void _artboard_changed();
protected:
    static void _bind_methods();

    bool _set(const godot::StringName &p_name, const godot::Variant &p_value);
    bool _get(const godot::StringName &p_name, godot::Variant &r_ret) const;
    void _get_property_list(godot::List<godot::PropertyInfo> *p_list) const;

    void _notification(int p_what);
public:

    void set_resource(const godot::Ref<RiveResource>& p_resource);
    [[nodiscard]] godot::Ref<RiveResource> get_resource() const;

    void set_artboard(const godot::String& p_name);
    [[nodiscard]] godot::String get_artboard() const;

    void _ready() override;

    void _enter_tree() override;
    void _exit_tree() override;

    void _process(double p_delta) override;
    void _draw() override;
};


#endif //RIVE_GODOT_RIVE_CONTROL_H
