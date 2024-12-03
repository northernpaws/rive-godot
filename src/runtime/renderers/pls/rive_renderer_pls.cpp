

#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/os.hpp>

#include <rive/pls/gl/pls_render_context_gl_impl.hpp>

#include "rive_renderer_pls.h"


void RiveRendererPLS::_bind_methods() {

}

godot::Error RiveRendererPLS::initialize() {
    if (m_backend == PLSBackend::OPENGL) {
        // Make a new PLS context using the OpenGL implementation.
        godot::UtilityFunctions::print("Creating PLS OpenGL context");
        m_pls_render_context = rive::pls::PLSRenderContextGLImpl::MakeContext();
        ERR_FAIL_NULL_V_MSG(m_pls_render_context, godot::FAILED, "Failed to create PLS OpenGL context.");
    }

    return godot::OK;
}

godot::Ref<RiveRenderTarget> RiveRendererPLS::create_render_target(godot::Point2 p_size) {

    return {};
}

rive::Factory *RiveRendererPLS::_get_factory() const {
    static rive::NoOpFactory noop;
    ERR_FAIL_NULL_V_MSG(m_pls_render_context, &noop, "Cannot return a factory for a null PLSRenderContext. Returning a NoOp factory instead.");
    return m_pls_render_context.get();
}


