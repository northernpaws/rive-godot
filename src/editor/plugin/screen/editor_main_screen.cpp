

#include "editor_main_screen.h"

#include <godot_cpp/classes/h_split_container.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/array.hpp>

godot::TypedArray<godot::String> walk_dir(const godot::String& dir_path) {
    godot::TypedArray<godot::String> paths;
    auto dir = godot::DirAccess::open(dir_path);

    if (!dir.is_valid()) {
        return paths;
    }

    if (auto err = dir->list_dir_begin(); err) {
        ERR_PRINT_ED("error opening project dir");
    }

    auto file_path = dir->get_next();
    while (file_path != "") {

        if (dir->current_is_dir()) {
            paths.append_array(walk_dir(file_path));
        } else {
            paths.append(file_path);
        }

        file_path = dir->get_next();
    }

    return paths;
}

RiveEditorMainScreen::RiveEditorMainScreen() : godot::Control::Control() {
    auto riv_files = walk_dir("res://");

    // Ensure the main screen is clipping, otherwise controls can overflow over the rest of the editor UI.
    set_clip_contents(true);
    set_anchors_preset(PRESET_FULL_RECT);
    set_v_size_flags(SIZE_EXPAND_FILL); // important! otherwise the control doesn't fill the screen.

    // Wrap the main screen in a margin so that it's got some padding from the edges.
    auto margin_wrapper = memnew(godot::MarginContainer); {
        margin_wrapper->set_anchors_preset(PRESET_FULL_RECT);
        margin_wrapper->add_theme_constant_override("margin_left", 5);
        margin_wrapper->add_theme_constant_override("margin_top", 5);
        margin_wrapper->add_theme_constant_override("margin_right", 5);
        margin_wrapper->add_theme_constant_override("margin_bottom", 5);

        // Main split box
        auto split_box = memnew(godot::HSplitContainer); {
            split_box->set_split_offset(200);

            // Construct the sidebar
            sidebar = memnew(godot::VBoxContainer); {
                auto sidebar_container = memnew(godot::VBoxContainer); {
                    for(int i = 0; i < riv_files.size(); i++) {
                        if (riv_files[i].stringify().rfind(".riv") != -1) {
                            auto file_label = memnew(godot::Label);
                            file_label->set_text(riv_files[i].stringify().get_file());
                            sidebar->add_child(file_label);
                        }
                    }
                } sidebar->add_child(sidebar_container);
                auto test_text = memnew(godot::Label);
                test_text->set_text("This is the sidebar!");
                sidebar->add_child(test_text);
            } split_box->add_child(sidebar); // add the sidebar

            // Construct the main content area
            auto content = memnew(godot::VBoxContainer); {
                auto test_text = memnew(godot::Label);
                test_text->set_text("This is the content!");
                content->add_child(test_text);
            } split_box->add_child(content); // add the content area
        } margin_wrapper->add_child(split_box); // add main split box
    } add_child(margin_wrapper); // add margin wrapper
}

RiveEditorMainScreen::~RiveEditorMainScreen() {
    godot::Control::~Control();
}
