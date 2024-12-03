

#ifndef RIVE_GODOT_RIVE_FILE_H
#define RIVE_GODOT_RIVE_FILE_H

#include <godot_cpp/classes/ref_counted.hpp>

#include <rive/file.hpp>

#include "resources/rive_resource.h"

#include "runtime/rive_artboard.h"
#include "rive_render_target.h"

/**
 * Encapsulates a rive::File that's been imported for the active Rive renderer.
 */
class RiveFile : public godot::RefCounted {
    GDCLASS(RiveFile, godot::RefCounted)

    /// Source resource the file was instantiated from.
    godot::Ref<RiveResource> m_resource;

    /// The imported rive file in memory.
    std::unique_ptr<rive::File> m_file = nullptr;
protected:
    static void _bind_methods();
public:
    static godot::Ref<RiveFile> load(const godot::Ref<RiveResource>& p_resource);

    RiveFile();
    ~RiveFile();

    [[nodiscard]] uint get_artboard_count() const;
    [[nodiscard]] godot::String get_default_artboard_name() const;
    [[nodiscard]] godot::String get_artboard_name(uint p_index) const;
    [[nodiscard]] godot::TypedArray<godot::String> get_artboard_names() const;

    [[nodiscard]] godot::Ref<RiveArtboard> instantiate_default_artboard () const;
    [[nodiscard]] godot::Ref<RiveArtboard> instantiate_artboard (uint p_index) const;
    [[nodiscard]] godot::Ref<RiveArtboard> instantiate_artboard_named (const godot::String& p_name) const;
};


#endif //RIVE_GODOT_RIVE_FILE_H
