

#ifndef RIVE_GODOT_RIVE_RESOURCE_H
#define RIVE_GODOT_RIVE_RESOURCE_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_format_loader.hpp>

/**
 * Resource storage wrapper for a .riv file loaded from disk or remotely.
 *
 * This does not directly call the rive importer or store the C++ representation of
 * the source .riv file. This class ONLY stores the .riv file bytes and metadata
 * for storing and loading the source file.
 *
 * See runtime/rive_file.h for the actual loader and rive::File encapsulation.
 */
class RiveResource : public godot::Resource {
    GDCLASS(RiveResource, godot::Resource)

    /// The underlying .riv file data.
    godot::PackedByteArray m_bytes;
protected:
    static void _bind_methods();
public:
    /**
     * Retrieve the raw bytes for the .riv resource.
     *
     * @return Bytes for the .riv file.
     */
    [[nodiscard]] godot::PackedByteArray get_bytes() const;

    /**
     * Set the raw .riv bytes for the resource.
     *
     * @param p_bytes valid bytes from a .riv file
     */
    void set_bytes(const godot::PackedByteArray& p_bytes);
};


/**
 * A resource loader for loading .riv files as Godot resources.
 */
class RiveResourceLoader : public godot::ResourceFormatLoader {
    GDCLASS(RiveResourceLoader, godot::ResourceFormatLoader)
protected:
    static void _bind_methods() {}
public:
    godot::PackedStringArray _get_recognized_extensions() const override;
    bool _handles_type(const godot::StringName &p_type) const override;
    godot::String _get_resource_type(const godot::String &p_path) const override;
    godot::Variant _load(const godot::String &p_path, const godot::String &p_original_path, bool p_use_sub_threads, int32_t p_cache_mode) const override;
};

#endif //RIVE_GODOT_RIVE_RESOURCE_H
