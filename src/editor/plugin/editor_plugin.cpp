

#include "editor_plugin.h"

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/file_system_dock.hpp>
#include <godot_cpp/classes/editor_resource_preview.hpp>

RiveEditorPlugin::RiveEditorPlugin() { };

RiveEditorPlugin::~RiveEditorPlugin() { };

godot::String RiveEditorPlugin::_get_plugin_name() const {
    return "Rive";
}

bool RiveEditorPlugin::_has_main_screen() const {
    return true;
}

void RiveEditorPlugin::_enter_tree() {
    Node::_enter_tree();

    resource_inspector_plugin.instantiate();
    add_inspector_plugin(resource_inspector_plugin);

    drawable_inspector_plugin.instantiate();
    add_inspector_plugin(drawable_inspector_plugin);

    import_plugin.instantiate();
    add_import_plugin(import_plugin);

    resource_preview_generator.instantiate();
    get_editor_interface()->get_resource_previewer()->add_preview_generator(resource_preview_generator);

    resource_tooltip_plugin.instantiate();
    get_editor_interface()->get_file_system_dock()->add_resource_tooltip_plugin(resource_tooltip_plugin);

    main_screen_instance = memnew(RiveEditorMainScreen);
    get_editor_interface()->get_editor_main_screen()->add_child(main_screen_instance);
    _make_visible(false); // initially hide it until the editor calls for it.
}

void RiveEditorPlugin::_exit_tree() {
    Node::_exit_tree();

    if (main_screen_instance) {
        main_screen_instance->queue_free();
    }

    if(resource_tooltip_plugin.is_valid()) {
        get_editor_interface()->get_file_system_dock()->remove_resource_tooltip_plugin(resource_tooltip_plugin);
        resource_tooltip_plugin.unref();
    }

    if (resource_preview_generator.is_valid()) {
        get_editor_interface()->get_resource_previewer()->remove_preview_generator(resource_preview_generator);
        resource_preview_generator.unref();
    }

    if(import_plugin.is_valid()) {
        remove_import_plugin(import_plugin);
        import_plugin.unref();
    }

    if(drawable_inspector_plugin.is_valid()) {
        remove_inspector_plugin(drawable_inspector_plugin);
        drawable_inspector_plugin.unref();
    }

    if(resource_inspector_plugin.is_valid()) {
        remove_inspector_plugin(resource_inspector_plugin);
        resource_inspector_plugin.unref();
    }
}

void RiveEditorPlugin::_make_visible(bool visible) {
    EditorPlugin::_make_visible(visible);

    if (main_screen_instance) {
        main_screen_instance->set_visible(visible);
    }
}
