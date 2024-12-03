

#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_file_system.hpp>
#include <godot_cpp/classes/editor_file_system_directory.hpp>

#include "rive_drawable_picker_dialog.h"

RiveDrawablePickerDialog::RiveDrawablePickerDialog() {
    set_title("Select a Drawable");
    content = memnew(godot::VBoxContainer);
    add_child(content);

    godot::HBoxContainer* filter_hbc = memnew(godot::HBoxContainer);
    content->add_child(filter_hbc);

    filter = memnew(godot::LineEdit);
    filter->set_h_size_flags(godot::Control::SIZE_EXPAND_FILL);
    filter->set_placeholder("Filter Drawables");
    filter->set_clear_button_enabled(true);
    filter->add_theme_constant_override("minimum_character_width", 0);
//    filter->connect("text_changed", callable_mp(this, &SceneTreeDialog::_filter_changed));
    filter_hbc->add_child(filter);


//    godot::EditorFileSystemDirectory *efsd = godot::EditorInterface::get_singleton()->get_resource_filesystem();


//    tree = memnew(SceneTreeEditor(false, false, true));
//    tree->set_v_size_flags(Control::SIZE_EXPAND_FILL);
//    tree->get_scene_tree()->connect("item_activated", callable_mp(this, &SceneTreeDialog::_select));
//    // Initialize button state, must be done after the tree has been created to update its 'show_all_nodes' flag.
//    // This is also done before adding the tree to the content to avoid triggering unnecessary tree filtering.
//    show_all_nodes->set_pressed(EditorSettings::get_singleton()->get_project_metadata("editor_metadata", "show_all_nodes_for_node_selection", false));
//    content->add_child(tree);

//    // Disable the OK button when no node is selected.
//    get_ok_button()->set_disabled(!tree->get_selected());
//    tree->connect("node_selected", callable_mp(this, &SceneTreeDialog::_selected_changed));
}

void RiveDrawablePickerDialog::_build_search_cache(godot::EditorFileSystemDirectory *p_efsd) {
//    for (int i = 0; i < p_efsd->get_subdir_count(); i++) {
//        _build_search_cache(p_efsd->get_subdir(i));
//    }
//
//    Vector<String> base_types = base_type.split(",");
//    for (int i = 0; i < p_efsd->get_file_count(); i++) {
//        godot::String file = p_efsd->get_file_path(i);
//        godot::String engine_type = p_efsd->get_file_type(i);
//        godot::String script_type = p_efsd->get_file_resource_script_class(i);
//        godot::String actual_type = script_type.is_empty() ? engine_type : script_type;
//
//        // Iterate all possible base types.
//        for (godot::String &parent_type : base_types) {
//            if (godot::ClassDB::is_parent_class(engine_type, parent_type) || godot::EditorNode::get_editor_data().script_class_is_parent(script_type, parent_type)) {
//                files.push_back(file.substr(6, file.length()));
//
//                // Store refs to used icons.
//                godot::String ext = file.get_extension();
//                if (!icons.has(ext)) {
//                    icons.insert(ext, godot::EditorNode::get_singleton()->get_class_icon(actual_type, "Object"));
//                }
//
//                // Stop testing base types as soon as we got a match.
//                break;
//            }
//        }
//    }
}
