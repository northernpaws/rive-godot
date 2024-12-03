

#ifndef RIVE_GODOT_RIVE_RENDERER_H
#define RIVE_GODOT_RIVE_RENDERER_H

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <utils/no_op_factory.hpp>
#include "rive_render_target.h"

/**
 * Base class for implementing Rive renderer backends.
 */
class RiveRenderer : public godot::RefCounted {
    GDCLASS(RiveRenderer, godot::RefCounted)

    static godot::Ref<RiveRenderer> m_singleton;
protected:
    static void _bind_methods();

//    template <class T, class B>
//    static void register_virtuals() {
//        godot::RefCounted::register_virtuals<T, B>();
//    }
public:
    inline static godot::Ref<RiveRenderer> get_singleton() {
        return m_singleton;
    }

    inline static void set_singleton(const godot::Ref<RiveRenderer>& p_singleton) {
        m_singleton = p_singleton;
    }

    RiveRenderer() = default;
    virtual ~RiveRenderer() = default;

    virtual godot::Error initialize() {
        ERR_PRINT("RiveRenderer did not implement initialize.");
        return godot::FAILED;
    };

    virtual godot::Ref<RiveRenderTarget> create_render_target(godot::Point2 p_size);

    [[nodiscard]] virtual rive::Factory* _get_factory() const {
        static rive::NoOpFactory factory;
        ERR_FAIL_V_MSG(&factory, "RiveRenderer did not implement _get_factory, returning NoOpFactory.");
    };
};


#endif //RIVE_GODOT_RIVE_RENDERER_H
