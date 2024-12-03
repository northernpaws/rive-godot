

#ifndef RIVE_GODOT_RIVE_RENDER_TARGET_H
#define RIVE_GODOT_RIVE_RENDER_TARGET_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/texture.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <rive/factory.hpp>
#include "rive_texture.h"
#include "rive_animation.h"
#include "rive_state_machine.h"

class RiveArtboard;

/**
 * A RiveRenderTarget serves as a binding between a Godot Texture (for displaying the rendered graphics)
 *  and the Rive rendering backend methods (i.e. managing alignment, texture resizing, etc.);
 *
 * The size of the render target is calculate the alignment of the rendered Rive objects within the space.
 */
class RiveRenderTarget : public godot::RefCounted {
    GDCLASS(RiveRenderTarget, godot::RefCounted)
protected:
    godot::Point2 m_size;

    godot::Ref<RiveTexture> m_texture = nullptr;

    static void _bind_methods();

    explicit RiveRenderTarget(godot::Point2 p_size) : m_size(p_size), m_texture(memnew(RiveTexture())) {};
public:

    RiveRenderTarget() = default;
    ~RiveRenderTarget() override = default;

    virtual godot::Error _initialize() {
        ERR_FAIL_V_MSG(godot::FAILED, "RiveRenderTarget has not implemented _initialize");
    };

    virtual void draw_artboard(const godot::Ref<RiveArtboard> &p_artboard) {
        ERR_FAIL_MSG("RiveRenderTarget has not implemented _draw_artboard");
    };

    virtual void draw_animation(const godot::Ref<RiveAnimation> &p_animation) {
        ERR_FAIL_MSG("RiveRenderTarget has not implemented _draw_animation");
    };

    virtual void draw_state_machine(const godot::Ref<RiveStateMachine> &p_state_machine) {
        ERR_FAIL_MSG("RiveRenderTarget has not implemented _draw_state_machine");
    };

    virtual void resize(godot::Point2 p_size) {
        ERR_FAIL_MSG("RiveRenderTarget has not implemented resize");
    };

    [[nodiscard]] virtual real_t get_width() const {
        return m_size.width;
    };

    [[nodiscard]] virtual real_t get_height() const {
        return m_size.height;
    };

    virtual godot::Point2 get_size() const {
        return m_size;
    }

    [[nodiscard]] virtual godot::Ref<RiveTexture> get_texture() const { return m_texture; };
};


#endif //RIVE_GODOT_RIVE_RENDER_TARGET_H
