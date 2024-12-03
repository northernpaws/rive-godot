

#ifndef RIVE_GODOT_RIVE_ARTBOARD_H
#define RIVE_GODOT_RIVE_ARTBOARD_H

#include <godot_cpp/classes/ref_counted.hpp>

#include <rive/artboard.hpp>

#include "runtime/rive_state_machine.h"
#include "rive_animation.h"

/**
 * Encapsulates a rive::Artboard instantiated from a RiveFile.
 */
class RiveArtboard : public godot::RefCounted {
    GDCLASS(RiveArtboard, godot::RefCounted)

    std::unique_ptr<rive::ArtboardInstance> m_artboard_instance = nullptr;
protected:
    static void _bind_methods();

    RiveArtboard();
public:
    explicit RiveArtboard(std::unique_ptr<rive::ArtboardInstance> p_artboard);
    ~RiveArtboard() override;

    // internal getter and setters
    void _set_artboard_instance(std::unique_ptr<rive::ArtboardInstance> p_artboard);
    rive::ArtboardInstance* _get_rive_artboard_instance();

    [[nodiscard]] godot::StringName get_name() const;

    [[nodiscard]] float get_width() const;
    [[nodiscard]] float get_height() const;

    // State machines
    [[nodiscard]] uint get_state_machine_count() const;
    [[nodiscard]] godot::String get_state_machine_name(uint p_index) const;
    [[nodiscard]] godot::Ref<RiveStateMachine> instantiate_state_machine(uint p_index) const;
    [[nodiscard]] godot::Ref<RiveStateMachine> instantiate_state_machine_named(godot::String p_name) const;

    // Animations
    [[nodiscard]] uint get_animation_count() const;
    [[nodiscard]] godot::String get_animation_name(uint p_index) const;
    [[nodiscard]] godot::Ref<RiveAnimation> instantiate_animation(uint p_index) const;
    [[nodiscard]] godot::Ref<RiveAnimation> instantiate_animation_named(godot::String p_name) const;

    void advance(double p_delta);

    // internal runtime methods
    [[nodiscard]] rive::AABB _get_rive_bounds() const;
};


#endif //RIVE_GODOT_RIVE_ARTBOARD_H
