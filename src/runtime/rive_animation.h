

#ifndef RIVE_GODOT_RIVE_ANIMATION_H
#define RIVE_GODOT_RIVE_ANIMATION_H

#include <godot_cpp/classes/ref_counted.hpp>

#include <rive/animation/linear_animation_instance.hpp>

/**
 * Encapsulates an animation from a Rive file in a Godot interface.
 */
class RiveAnimation : public godot::RefCounted {
    GDCLASS(RiveAnimation, godot::RefCounted)

    std::unique_ptr<rive::LinearAnimationInstance> m_animation = nullptr;
protected:
    static void _bind_methods();

    RiveAnimation() = default;
public:
    explicit RiveAnimation(std::unique_ptr<rive::LinearAnimationInstance> p_animation);

    // internal methods for renderer
    [[nodiscard]] rive::LinearAnimationInstance* _get_rive_animation_instance() const;
    [[nodiscard]] rive::AABB _get_rive_bounds() const;
};


#endif //RIVE_GODOT_RIVE_ANIMATION_H
