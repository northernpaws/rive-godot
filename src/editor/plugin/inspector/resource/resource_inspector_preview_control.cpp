

#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/option_button.hpp>

#include "resource_inspector_preview_control.h"

RiveResourceInspectorPreviewControl::RiveResourceInspectorPreviewControl() {

}

RiveResourceInspectorPreviewControl::RiveResourceInspectorPreviewControl(godot::Ref<RiveResource> p_resource) {
    auto vbox = memnew(godot::VBoxContainer);
    vbox->set_anchors_preset(PRESET_FULL_RECT);

    {
        auto controls_container = memnew(godot::HBoxContainer);

        auto artboard_select = memnew(godot::OptionButton);
        artboard_select->add_item("artboard1", 0);
        artboard_select->add_item("artboard2", 1);
        artboard_select->add_item("artboard3", 2);
        controls_container->add_child(artboard_select);

        vbox->add_child(controls_container);
    }

    {
        auto playback_area_container = memnew(godot::Control);
        playback_area_container->set_v_size_flags(SIZE_EXPAND_FILL);

        vbox->add_child(playback_area_container);
    }

    {
        auto playback_controls_container = memnew(godot::HBoxContainer);

        auto play_button = memnew(godot::Button);
        play_button->set_text("play");
        playback_controls_container->add_child(play_button);

        auto rewind_button = memnew(godot::Button);
        rewind_button->set_text("rewind");
        playback_controls_container->add_child(rewind_button);

        vbox->add_child(playback_controls_container);
    }

    add_child(vbox);
}

RiveResourceInspectorPreviewControl::~RiveResourceInspectorPreviewControl() {

}

