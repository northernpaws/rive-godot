

#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "editor_resource_tooltip_plugin.h"

#include "resources/rive_resource.h"

bool RiveEditorResourceTooltipPlugin::_handles(const godot::String &type) const {
    return type == RiveResource::get_class_static();
}

godot::Control* RiveEditorResourceTooltipPlugin::_make_tooltip_for_path(const godot::String &path, const godot::Dictionary &metadata,
                                                    godot::Control *base) const {

    // Extract metadata provided by the RiveResourcePreviewGenerator.
    godot::String default_artboard = metadata["default_artboard"];
    godot::String default_state_machine = metadata["default_state_machine"];

    uint32_t artboard_count = metadata["artboard_count"];

    auto* tooltip_control = memnew(godot::HBoxContainer);
    {
        {
            auto *preview_texture = memnew(godot::TextureRect);
            preview_texture->set_v_size_flags(godot::Control::SIZE_SHRINK_CENTER);

            // Request the thumbnail for the resource.
            //
            // See EditorResourcePreviewGenerator.
            request_thumbnail(path, preview_texture);

            tooltip_control->add_child(preview_texture);
        }

        // Adjust the alignment for the default tooltip control.
        //
        // This base includes things like the resource filename,
        // type, and size that are common across tooltips.
        auto *vb = godot::Object::cast_to<godot::VBoxContainer>(base);
        DEV_ASSERT(vb);
        vb->set_alignment(godot::BoxContainer::ALIGNMENT_CENTER);

        {
            auto *label_artboard_count = memnew(godot::Label);
            label_artboard_count->set_text(godot::vformat("%d artboards", artboard_count));
            vb->add_child(label_artboard_count);

            auto *label_artboard = memnew(godot::Label);
            label_artboard->set_text(vformat("Artboard: %s", default_artboard));
            vb->add_child(label_artboard);

            auto *label_state_machine = memnew(godot::Label);
            label_state_machine->set_text(vformat("State machine: %s", default_state_machine));
            vb->add_child(label_state_machine);
        }

        tooltip_control->add_child(vb);
    }
    // TODO: animate artboard playback on tree enter/exit?

    return tooltip_control;
}
