

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/wrapped.hpp>

#include "rive_drawable_property_editor_control.h"

#include "resources/rive_resource.h"


void RiveDrawablePropertyEditorControl::_resource_selected(const godot::Ref<RiveResource> &p_drawable, bool p_inspect) {
    // When the selected Rive file resource changes, display the sub-resource selector again.
    m_dialog->popup_centered_clamped(godot::Size2(350, 500));
}

void RiveDrawablePropertyEditorControl::_resource_changed(const godot::Ref<RiveResource> &p_drawable) {
    // When the selected Rive file resource changes, display the sub-resource selector again.
    m_dialog->popup_centered_clamped(godot::Size2(350, 500));
}


void RiveDrawablePropertyEditorControl::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("_on_button_pressed"), &RiveDrawablePropertyEditorControl::_on_button_pressed);
}

void RiveDrawablePropertyEditorControl::_on_button_pressed() {
    godot::UtilityFunctions::print("clicked drawable property button");
//    m_dialog->popup_centered_clamped(godot::Size2(350, 700));
}

RiveDrawablePropertyEditorControl::RiveDrawablePropertyEditorControl() {
    m_drawable_picker = memnew(EditorResourcePickerRiveResource);
    m_drawable_picker->set_base_type(RiveResource::get_class_static());
    m_drawable_picker->set_editable(true);
    m_drawable_picker->set_h_size_flags(godot::Control::SizeFlags::SIZE_EXPAND_FILL);
    add_child(m_drawable_picker);

    m_drawable_picker->connect("resource_selected", callable_mp(this, &RiveDrawablePropertyEditorControl::_resource_selected));
    m_drawable_picker->connect("resource_changed", callable_mp(this, &RiveDrawablePropertyEditorControl::_resource_changed));

    m_dialog = memnew(RiveDrawablePickerDialog);
    m_dialog->set_title("Pick a Artboard, Animation, or State Machine");

    add_child(m_dialog);

//    Vector<StringName> valid_types;
//    valid_types.push_back("Viewport");
//    scene_tree->set_valid_types(valid_types);
//    scene_tree->get_scene_tree()->set_show_enabled_subscene(true);
//
//    add_child(scene_tree);
//    scene_tree->connect("selected", callable_mp(this, &EditorPropertyResource::_viewport_selected));

//    m_property_button = memnew(godot::Button());
//    add_child(m_property_button);
//    add_focusable(m_property_button);
//    m_property_button->set_text("<empty>");
//
//    m_property_button->connect("pressed", callable_mp(this, &RiveDrawablePropertyEditorControl::_on_button_pressed));
}

void RiveDrawablePropertyEditorControl::_update_property() {
    // Read the current value from the property.
    godot::Ref<RiveDrawable> new_value = get_edited_object()->get(get_edited_property());
    if (new_value == m_current_value) {
        return;
    }

    m_current_value = new_value;

    if (m_current_value.is_valid() != m_drawable_picker->is_toggle_mode()) {
        m_drawable_picker->set_toggle_mode(m_current_value.is_valid());
    }

   /*

    if (m_current_value.is_valid()) {
        if (!sub_inspector) {
            sub_inspector = memnew(godot::EditorInspector);
//            sub_inspector->set_vertical_scroll_mode(ScrollContainer::SCROLL_MODE_DISABLED);
//            sub_inspector->set_use_doc_hints(true);
//
//            sub_inspector->set_sub_inspector(true);
//            sub_inspector->set_property_name_style(InspectorDock::get_singleton()->get_property_name_style());
//
//            sub_inspector->connect("property_keyed", callable_mp(this, &EditorPropertyResource::_sub_inspector_property_keyed));
//            sub_inspector->connect("resource_selected", callable_mp(this, &EditorPropertyResource::_sub_inspector_resource_selected));
//            sub_inspector->connect("object_id_selected", callable_mp(this, &EditorPropertyResource::_sub_inspector_object_id_selected));
//            sub_inspector->set_keying(is_keying());
//            sub_inspector->set_read_only(is_read_only());
//            sub_inspector->set_use_folding(is_using_folding());

            sub_inspector->set_mouse_filter(MOUSE_FILTER_STOP);
            add_child(sub_inspector);
            set_bottom_editor(sub_inspector);

            m_drawable_picker->set_toggle_pressed(true);
        }

        if (m_current_value.ptr() != sub_inspector->get_edited_object()) {
//            sub_inspector->edit(res.ptr());
        }
    }else {
        if (sub_inspector) {
            set_bottom_editor(nullptr);
            memdelete(sub_inspector);
            sub_inspector = nullptr;
//
//            if (opened_editor) {
//                EditorNode::get_singleton()->hide_unused_editors();
//                opened_editor = false;
//            }
        }
    }

    m_drawable_picker->set_edited_resource(m_current_value);
//
//    if (m_current_value.is_null()) {
//        m_property_button->set_text("<empty>");
//    } else {
//        m_property_button->set_text(new_value->get_path().get_file());
//    }


//    if (m_current_value.is_valid()) {
//        sub_inspector = memnew(godot::EditorInspector);
//        sub_inspector->set_vertical_scroll_mode(godot::ScrollContainer::SCROLL_MODE_DISABLED);
////        sub_inspector->set_use_doc_hints(true);
////
////        sub_inspector->set_sub_inspector(true);
////        sub_inspector->set_property_name_style(InspectorDock::get_singleton()->get_property_name_style());
////
////        sub_inspector->connect("property_keyed", callable_mp(this, &EditorPropertyResource::_sub_inspector_property_keyed));
////        sub_inspector->connect("resource_selected", callable_mp(this, &EditorPropertyResource::_sub_inspector_resource_selected));
////        sub_inspector->connect("object_id_selected", callable_mp(this, &EditorPropertyResource::_sub_inspector_object_id_selected));
////        sub_inspector->set_keying(is_keying());
////        sub_inspector->set_read_only(is_read_only());
////        sub_inspector->set_use_folding(is_using_folding());
//
//        sub_inspector->set_mouse_filter(MOUSE_FILTER_STOP);
//        add_child(sub_inspector);
//        set_bottom_editor(sub_inspector);
//    }*/
}

void RiveDrawablePropertyEditorControl::_set_read_only(bool read_only) {
    EditorProperty::_set_read_only(read_only);
}





