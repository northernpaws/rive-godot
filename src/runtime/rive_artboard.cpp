

#include <godot_cpp/variant/utility_functions.hpp>
#include "rive_artboard.h"

void RiveArtboard::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("get_width"), &RiveArtboard::get_width);
    godot::ClassDB::bind_method(godot::D_METHOD("get_height"), &RiveArtboard::get_height);

    godot::ClassDB::bind_method(godot::D_METHOD("get_state_machine_count"), &RiveArtboard::get_state_machine_count);
    godot::ClassDB::bind_method(godot::D_METHOD("get_state_machine_name", "p_index"), &RiveArtboard::get_state_machine_name);

    godot::ClassDB::bind_method(godot::D_METHOD("instantiate_state_machine", "p_index"), &RiveArtboard::instantiate_state_machine);
    godot::ClassDB::bind_method(godot::D_METHOD("instantiate_state_machine_named", "p_name"), &RiveArtboard::instantiate_state_machine_named);

    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::STRING_NAME, "width",
                                     godot::PROPERTY_HINT_NONE, "",
                                     godot::PROPERTY_USAGE_READ_ONLY),
                 "", "get_width");

    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::STRING_NAME, "height",
                                     godot::PROPERTY_HINT_NONE, "",
                                     godot::PROPERTY_USAGE_READ_ONLY),
                 "", "get_height");
}

RiveArtboard::RiveArtboard() {
    WARN_PRINT("RiveArtboard default constructor called, ensure _set_artboard is called after.");
};

RiveArtboard::RiveArtboard(std::unique_ptr<rive::ArtboardInstance> p_artboard_instance) {
    _set_artboard_instance(std::move(p_artboard_instance));
}

RiveArtboard::~RiveArtboard() = default;

void RiveArtboard::_set_artboard_instance(std::unique_ptr<rive::ArtboardInstance> p_artboard_instance) {
    m_artboard_instance = std::move(p_artboard_instance);
}

rive::ArtboardInstance* RiveArtboard::_get_rive_artboard_instance() {
    return m_artboard_instance.get();
}

godot::StringName RiveArtboard::get_name() const {
    return {m_artboard_instance->name().c_str()};
}

float RiveArtboard::get_width() const {
    ERR_FAIL_NULL_V_MSG(m_artboard_instance, 0, "m_artboard_instance is null");
    return m_artboard_instance->width();
}

float RiveArtboard::get_height() const {
    ERR_FAIL_NULL_V_MSG(m_artboard_instance, 0, "m_artboard_instance is null");
    return m_artboard_instance->height();
}

uint RiveArtboard::get_state_machine_count() const {
    ERR_FAIL_NULL_V_MSG(m_artboard_instance, {}, "m_artboard_instance is null");
    return m_artboard_instance->stateMachineCount();
}

godot::String RiveArtboard::get_state_machine_name(uint p_index) const {
    ERR_FAIL_NULL_V_MSG(m_artboard_instance, {}, "m_artboard_instance is null");
    ERR_FAIL_NULL_V_MSG(m_artboard_instance->stateMachine(p_index), {}, "state machine p_index does not exist");
    return {m_artboard_instance->stateMachineNameAt(p_index).c_str()};
}

godot::Ref<RiveStateMachine> RiveArtboard::instantiate_state_machine(uint p_index) const {
    ERR_FAIL_NULL_V_MSG(m_artboard_instance, {}, "m_artboard_instance is null");
    ERR_FAIL_NULL_V_MSG(m_artboard_instance->stateMachine(p_index), {}, "state machine p_index does not exist");

    // Initialize the state machine object with a state machine instance.
    godot::Ref<RiveStateMachine> state_machine = memnew(RiveStateMachine(m_artboard_instance->stateMachineAt(p_index)));

    return state_machine;
}

godot::Ref<RiveStateMachine> RiveArtboard::instantiate_state_machine_named(godot::String p_name) const {
    ERR_FAIL_NULL_V_MSG(m_artboard_instance, {}, "m_artboard_instance is null");

    auto name = std::string(p_name.utf8().ptr());
    ERR_FAIL_NULL_V_MSG(m_artboard_instance->stateMachine(name), {}, "state machine p_index does not exist");

    // Initialize the artboard object with an artboard instance.
    godot::Ref<RiveStateMachine> state_machine = memnew(RiveStateMachine(m_artboard_instance->stateMachineNamed(name)));

    return state_machine;
}

uint RiveArtboard::get_animation_count() const {
    ERR_FAIL_NULL_V_MSG(m_artboard_instance, {}, "m_artboard_instance is null");
    return m_artboard_instance->animationCount();
}

godot::String RiveArtboard::get_animation_name(uint p_index) const {
    ERR_FAIL_NULL_V_MSG(m_artboard_instance, {}, "m_artboard_instance is null");
    ERR_FAIL_NULL_V_MSG(m_artboard_instance->animation(p_index), {}, "animation p_index does not exist");
    return {m_artboard_instance->animationNameAt(p_index).c_str()};
}

godot::Ref<RiveAnimation> RiveArtboard::instantiate_animation(uint p_index) const {
    ERR_FAIL_NULL_V_MSG(m_artboard_instance, {}, "m_artboard_instance is null");
    ERR_FAIL_NULL_V_MSG(m_artboard_instance->animation(p_index), {}, "animation p_index does not exist");

    // Initialize the animation object with an animation instance.
    godot::Ref<RiveAnimation> animation = memnew(RiveAnimation(m_artboard_instance->animationAt(p_index)));

    return animation;
}

godot::Ref<RiveAnimation> RiveArtboard::instantiate_animation_named(godot::String p_name) const {
    ERR_FAIL_NULL_V_MSG(m_artboard_instance, {}, "m_artboard_instance is null");

    auto name = std::string(p_name.utf8().ptr());
    ERR_FAIL_NULL_V_MSG(m_artboard_instance->animation(name), {}, "animation p_index does not exist");

    // Initialize the artboard object with an artboard instance.
    godot::Ref<RiveAnimation> animation = memnew(RiveAnimation(m_artboard_instance->animationNamed(name)));

    return animation;
}


void RiveArtboard::advance(double p_delta) {
    ERR_FAIL_NULL_MSG(m_artboard_instance, "Cannot advance Artboard with null artboard instance.");

    if (m_artboard_instance) {
//        godot::UtilityFunctions::print("[RiveArtboard] advancing by", p_delta);
        m_artboard_instance->advance(p_delta);
    }
}

rive::AABB RiveArtboard::_get_rive_bounds() const {
    return m_artboard_instance->bounds();
}






