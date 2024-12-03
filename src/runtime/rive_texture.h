

#ifndef RIVE_GODOT_RIVE_TEXTURE_H
#define RIVE_GODOT_RIVE_TEXTURE_H


#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/image_texture.hpp>

/**
 * Texture2D wrapper for textures managed by a Rive renderer.
 *
 * Follows a similar pattern to ImageTexture or ViewportTexture proxy resources in Godot engine.
 *
 * The texture RID for a RiveTexture can always be assumed to be accessible and drawable on the main Godot RenderingDevice.
 */
class RiveTexture : public godot::ImageTexture {
    GDCLASS(RiveTexture, godot::ImageTexture)
protected:
    static void _bind_methods();
public:
    RiveTexture() = default;
    ~RiveTexture() override = default;
};


#endif //RIVE_GODOT_RIVE_TEXTURE_H
