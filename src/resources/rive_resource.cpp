

#include <godot_cpp/classes/file_access.hpp>
#include "rive_resource.h"

void RiveResource::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("set_bytes", "p_bytes"), &RiveResource::set_bytes);
    godot::ClassDB::bind_method(godot::D_METHOD("get_bytes"), &RiveResource::get_bytes);

    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::PACKED_BYTE_ARRAY, "bytes",
                                     godot::PropertyHint::PROPERTY_HINT_NONE, "",
                                     godot::PropertyUsageFlags::PROPERTY_USAGE_STORAGE),
                 "set_bytes", "get_bytes");
}

godot::PackedByteArray RiveResource::get_bytes() const {
    return m_bytes;
}

void RiveResource::set_bytes(const godot::PackedByteArray& p_bytes) {
    m_bytes = p_bytes;
}


godot::PackedStringArray RiveResourceLoader::_get_recognized_extensions() const {
    godot::PackedStringArray extensions;
    extensions.push_back("riv");

    return extensions;
}

bool RiveResourceLoader::_handles_type(const godot::StringName &p_type) const {
    return p_type == RiveResource::get_class_static();
}

godot::String RiveResourceLoader::_get_resource_type(const godot::String &p_path) const  {
    return p_path.get_extension().to_lower() == "riv" ? "RiveFile": ""; // RiveFile::get_class_static(); //
}

godot::Variant RiveResourceLoader::_load(const godot::String &p_path, const godot::String &p_original_path, bool p_use_sub_threads, int32_t p_cache_mode) const  {
    godot::Ref<RiveResource> resource = memnew(RiveResource);

    auto bytes = godot::FileAccess::get_file_as_bytes(p_path);
    ERR_FAIL_COND_V_MSG(bytes.is_empty(), godot::FileAccess::get_open_error(), "failed to open .riv file for reading");
    resource->set_bytes(bytes);

    return resource;
}