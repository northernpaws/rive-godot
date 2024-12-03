

#include "rive_animation.h"

void RiveAnimation::_bind_methods() {

}

RiveAnimation::RiveAnimation(std::unique_ptr<rive::LinearAnimationInstance> p_animation) : m_animation(std::move(p_animation)) {

}

rive::LinearAnimationInstance *RiveAnimation::_get_rive_animation_instance() const {
    return m_animation.get();
}

rive::AABB RiveAnimation::_get_rive_bounds() const {
    return m_animation->bounds();
}
