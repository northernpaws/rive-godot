

#include "rive_state_machine.h"

void RiveStateMachine::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("get_input_count"), &RiveStateMachine::get_input_count);

    godot::ClassDB::bind_method(godot::D_METHOD("advance", "p_seconds"), &RiveStateMachine::advance);

    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::STRING_NAME, "input_count",
                                     godot::PROPERTY_HINT_NONE, "",
                                     godot::PROPERTY_USAGE_READ_ONLY),
                 "", "get_input_count");
}

RiveStateMachine::RiveStateMachine() {
    WARN_PRINT("RiveStateMachine default constructor called, ensure _set_state_machine is called after.");
};

RiveStateMachine::RiveStateMachine(std::unique_ptr<rive::StateMachineInstance> p_state_machine_instance) {
    m_state_machine_instance = std::move(p_state_machine_instance);
}

RiveStateMachine::~RiveStateMachine() = default;


uint RiveStateMachine::get_input_count() const {
    ERR_FAIL_NULL_V_MSG(m_state_machine_instance, {}, "Cannot retrieve the input count for a null state machine.");
    return m_state_machine_instance->inputCount();
}

bool RiveStateMachine::advance(float p_seconds) {
    ERR_FAIL_NULL_V_MSG(m_state_machine_instance, {}, "Cannot advance a null state machine.");
    return m_state_machine_instance->advance(p_seconds);
}

void RiveStateMachine::_set_state_machine_instance(std::unique_ptr<rive::StateMachineInstance> p_state_machine_instance) {
    m_state_machine_instance = std::move(p_state_machine_instance);
}

rive::StateMachineInstance* RiveStateMachine::_get_rive_state_machine_instance() {
    return m_state_machine_instance.get();
}

rive::AABB RiveStateMachine::_get_rive_bounds() const {
    ERR_FAIL_NULL_V_MSG(m_state_machine_instance, {}, "Cannot retrieve bounds for a null state machine.");
    return m_state_machine_instance->bounds();
}
