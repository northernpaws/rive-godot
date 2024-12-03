

#ifndef RIVE_GODOT_EDITOR_MAIN_SCREEN_H
#define RIVE_GODOT_EDITOR_MAIN_SCREEN_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

class RiveEditorMainScreen : public godot::Control {
    GDCLASS(RiveEditorMainScreen, godot::Control)

    godot::VBoxContainer* sidebar = nullptr;
protected:
    static void _bind_methods() {}
public:
    RiveEditorMainScreen();
    ~RiveEditorMainScreen();
};


#endif //RIVE_GODOT_EDITOR_MAIN_SCREEN_H
