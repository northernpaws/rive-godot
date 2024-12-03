

#ifndef RIVE_GODOT_RIVE_DRAWABLE_PICKER_DIALOG_H
#define RIVE_GODOT_RIVE_DRAWABLE_PICKER_DIALOG_H

#include <godot_cpp/classes/confirmation_dialog.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/editor_file_system_directory.hpp>

class RiveDrawablePickerDialog : public godot::ConfirmationDialog {
    GDCLASS(RiveDrawablePickerDialog, godot::ConfirmationDialog)

    godot::VBoxContainer* content = nullptr;
    // TODO: tree
    godot::LineEdit* filter = nullptr;

    void _build_search_cache(godot::EditorFileSystemDirectory *p_efsd);
protected:
    static void _bind_methods() {}
public:
    RiveDrawablePickerDialog();
};


#endif //RIVE_GODOT_RIVE_DRAWABLE_PICKER_DIALOG_H
