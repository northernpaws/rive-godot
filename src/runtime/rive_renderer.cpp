

#include "rive_renderer.h"

godot::Ref<RiveRenderer> RiveRenderer::m_singleton = nullptr;

void RiveRenderer::_bind_methods() {

}

godot::Ref<RiveRenderTarget> RiveRenderer::create_render_target(godot::Point2 p_size) {
    ERR_FAIL_V_MSG({}, "create_render_target has not been implemented");
}


