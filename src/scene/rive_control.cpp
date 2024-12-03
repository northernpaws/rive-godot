

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/rendering_server.hpp>

#include "rive_control.h"

#include "runtime/rive_renderer.h"
#include "runtime/renderers/skia/rive_renderer_skia.h"

void RiveControl::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("set_resource", "p_resource"), &RiveControl::set_resource);
    godot::ClassDB::bind_method(godot::D_METHOD("get_resource"), &RiveControl::get_resource);

    godot::ClassDB::bind_method(godot::D_METHOD("set_artboard", "p_artboard"), &RiveControl::set_artboard);
    godot::ClassDB::bind_method(godot::D_METHOD("get_artboard"), &RiveControl::get_artboard);

    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::OBJECT, "resource",
                                     godot::PropertyHint::PROPERTY_HINT_RESOURCE_TYPE, RiveResource::get_class_static(),
                                     godot::PropertyUsageFlags::PROPERTY_USAGE_DEFAULT,
                                     RiveResource::get_class_static()),
                 "set_resource", "get_resource");
}

void RiveControl::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_RESIZED: {
            update_minimum_size();

            if(m_render_target.is_valid()) {
                m_render_target->resize(get_size());
            }
        } break;
    }
}

void RiveControl::_resource_changed() {
    if (m_resource.is_valid()) {
        if (RiveRenderer::get_singleton() == nullptr) {
            godot::UtilityFunctions::print("Initializing new Skia Rive renderer!");
            // TODO: move to extension initialization somewhere
//            RiveRenderer::set_singleton(memnew(RiveRendererSkia(RiveRendererSkia::SkiaBackend::OPENGL)));
            RiveRenderer::set_singleton(memnew(RiveRendererSkia(RiveRendererSkia::SkiaBackend::VULKAN)));

            if (auto err = RiveRenderer::get_singleton()->initialize(); err != godot::OK) {
                ERR_PRINT("Failed to initialize Rive renderer!");
            }
        };

        godot::UtilityFunctions::print("[RiveControl] loading rive file from resource");
        m_file = RiveFile::load(m_resource); // TODO: use globally shared file instance as we use instances
        m_artboard = m_file->instantiate_default_artboard();
    } else {
        godot::UtilityFunctions::print("[RiveControl] invalid resource supplied!");
        m_file.unref();
        m_artboard.unref();
    }

    notify_property_list_changed();
}

void RiveControl::_artboard_changed() {
    notify_property_list_changed(); // updates _get and _get_property_list for the editor
    queue_redraw();
}

bool RiveControl::_set(const godot::StringName &p_name, const godot::Variant &p_value) {
    if (p_name == godot::StringName("artboard")) {
        if (m_file.is_valid()) {
            if (!m_file->get_artboard_names().has(p_value)) {
                if (m_file->get_artboard_count() > 0) {
                    set_artboard(m_file->get_default_artboard_name());
                } else {
                    set_artboard("");
                }
            } else {
                set_artboard(p_value);
            }
        } else {
            set_artboard("");
        }

        return true;
    }

    return false;
}

bool RiveControl::_get(const godot::StringName &p_name, godot::Variant &r_ret) const {
    if (p_name == godot::StringName("artboard")) {
        if (m_artboard.is_valid()) {
            r_ret = m_artboard->get_name();
        } else {
            if (m_file.is_valid()) {
                r_ret = m_file->get_default_artboard_name();
            } else {
                return "";
            }
        }

        return true;
    }

    return false;
}

void RiveControl::_get_property_list(godot::List<godot::PropertyInfo> *p_list) const {
    godot::String artboards_options;
    if (m_file.is_valid()) {
        for (uint i = 0; i < m_file->get_artboard_count(); i++) {
            if (i > 0) {
                artboards_options += ",";
            }
            artboards_options += m_file->get_artboard_name(i);
        }
    }

    p_list->push_back(godot::PropertyInfo(godot::Variant::STRING_NAME, "artboard",
                                          godot::PROPERTY_HINT_ENUM, artboards_options));
}

void RiveControl::set_resource(const godot::Ref<RiveResource>& p_resource) {
    if(m_resource == p_resource) {
        return;
    }

    if (m_resource.is_valid()) {
        m_resource->disconnect("changed", callable_mp(this, &RiveControl::_resource_changed));
    }

    m_resource = p_resource;

    if (m_resource.is_valid()) {
        m_resource->connect("changed", callable_mp(this, &RiveControl::_resource_changed));
    }

    _resource_changed();
}

godot::Ref<RiveResource> RiveControl::get_resource() const {
    return m_resource;
}

void RiveControl::set_artboard(const godot::String& p_name) {
    ERR_FAIL_NULL_MSG(m_file, "m_file is null");

    if (m_artboard.is_valid() && m_artboard->get_name() == p_name) {
        return;
    }

    if (m_artboard.is_valid()) {
        m_artboard->disconnect("changed", callable_mp(this, &RiveControl::_artboard_changed));
    }

    if (p_name != "") {
        m_artboard = m_file->instantiate_artboard_named(p_name);
    } else {
        m_artboard.unref();
    }

    if (m_artboard.is_valid()) {
        m_artboard->connect("changed", callable_mp(this, &RiveControl::_artboard_changed));
    }

    _artboard_changed();
}

godot::String RiveControl::get_artboard() const {
    ERR_FAIL_NULL_V_MSG(m_file, "", "m_file is null");
    ERR_FAIL_NULL_V_MSG(m_artboard, "", "m_artboard is null");
    return m_artboard->get_name();
}

void RiveControl::_ready() {

}

void RiveControl::_enter_tree() {
    if(m_render_target.is_null()) {
        godot::UtilityFunctions::print("[RiveControl] _enter_tree creating render target");
        m_render_target = RiveRenderer::get_singleton()->create_render_target(get_size());
    }

    queue_redraw();
}

void RiveControl::_exit_tree() {
    if(m_render_target.is_valid()) {
        godot::UtilityFunctions::print("[RiveControl] _exit_tree destroying render target");
        m_render_target.unref();
    }
}

void RiveControl::_process(double p_delta) {
    if(m_artboard.is_valid()) {
        m_artboard->advance(p_delta);

        // TODO: this probably shouldn't be here

        queue_redraw();
    }
}

void RiveControl::_draw() {
    auto rect = godot::Rect2(godot::Point2(), get_size());
    if (m_render_target.is_valid()) {
        if(m_artboard.is_valid()) {
            // Draw the artboard to the render target texture.
            m_render_target->draw_artboard(m_artboard);
        }

        // Always draw the texture so that we can see the rect,
        // even when we don't have an object to render to it.
        draw_texture_rect(m_render_target->get_texture(), rect, false);
    } else {
        // draw a pink box with an x so the control is visible and
        // signals that no valid render target has been configured.
        //
        // This is similar behavior to other nodes that display things
        // like viewport or camera textures that are invalid.
        draw_rect(rect, godot::Color(1, 0, 1, 1));
        draw_line(godot::Vector2(), get_size(), godot::Color(0,0,0,1));
        draw_line(godot::Vector2(0, get_size().height),
                  godot::Vector2(get_size().width, 0),
                  godot::Color(0,0,0,1));
    }

}







