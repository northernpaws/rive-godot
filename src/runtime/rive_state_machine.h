

#ifndef RIVE_GODOT_RIVE_STATE_MACHINE_H
#define RIVE_GODOT_RIVE_STATE_MACHINE_H

#include <godot_cpp/classes/ref_counted.hpp>

#include <rive/animation/state_machine_instance.hpp>

class RiveStateMachine : public godot::RefCounted {
    GDCLASS(RiveStateMachine, godot::RefCounted)

    std::unique_ptr<rive::StateMachineInstance> m_state_machine_instance = nullptr;
protected:
    static void _bind_methods();
public:
    RiveStateMachine();
    explicit RiveStateMachine(std::unique_ptr<rive::StateMachineInstance> p_state_machine_instance);
    ~RiveStateMachine() override;

    [[nodiscard]] uint get_input_count() const;
    // TODO: input retrieval
    // TODO: triggers
    // TODO: values

    bool advance(float p_seconds);

    // internal getter and setters
    void _set_state_machine_instance(std::unique_ptr<rive::StateMachineInstance> p_state_machine_instance);
    rive::StateMachineInstance* _get_rive_state_machine_instance();
    [[nodiscard]] rive::AABB _get_rive_bounds() const;

};


#endif //RIVE_GODOT_RIVE_STATE_MACHINE_H
