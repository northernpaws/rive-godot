

#ifndef RIVE_GODOT_EDITOR_RESOURCE_PREVIEW_GENERATOR_H
#define RIVE_GODOT_EDITOR_RESOURCE_PREVIEW_GENERATOR_H

#include <godot_cpp/classes/editor_resource_preview_generator.hpp>

class RiveEditorResourcePreviewGenerator : public godot::EditorResourcePreviewGenerator {
    GDCLASS(RiveEditorResourcePreviewGenerator, godot::EditorResourcePreviewGenerator)
protected:
    static void _bind_methods() {}
public:
    [[nodiscard]] bool _handles(const godot::String &type) const override;
    [[nodiscard]] godot::Ref<godot::Texture2D> _generate(const godot::Ref<godot::Resource> &resource, const godot::Vector2i &size, const godot::Dictionary &metadata) const override;
//    [[nodiscard]] godot::Ref<godot::Texture2D> _generate_from_path(const godot::String &path, const godot::Vector2i &size, const godot::Dictionary &metadata) const override;
    [[nodiscard]] bool _generate_small_preview_automatically() const override;
    [[nodiscard]] bool _can_generate_small_preview() const override;
};


#endif //RIVE_GODOT_EDITOR_RESOURCE_PREVIEW_GENERATOR_H
