

#include <string>

#include <utils/no_op_factory.hpp>

#include "rive_file.h"
#include "rive_render_target.h"
#include "rive_renderer.h"

void RiveFile::_bind_methods() {
    godot::ClassDB::bind_static_method(get_class_static(), godot::D_METHOD("load"), &RiveFile::load, "p_resource");

    godot::ClassDB::bind_method(godot::D_METHOD("get_artboard_count"), &RiveFile::get_artboard_count);
    godot::ClassDB::bind_method(godot::D_METHOD("get_default_artboard_name"), &RiveFile::get_default_artboard_name);
    godot::ClassDB::bind_method(godot::D_METHOD("get_artboard_name", "p_index"), &RiveFile::get_artboard_name);
    godot::ClassDB::bind_method(godot::D_METHOD("get_artboard_names"), &RiveFile::get_artboard_names);

    godot::ClassDB::bind_method(godot::D_METHOD("instantiate_default_artboard"), &RiveFile::instantiate_default_artboard);
    godot::ClassDB::bind_method(godot::D_METHOD("instantiate_artboard", "p_index"), &RiveFile::instantiate_artboard);
    godot::ClassDB::bind_method(godot::D_METHOD("instantiate_artboard_named", "p_name"), &RiveFile::instantiate_artboard_named);
}

godot::Ref<RiveFile> RiveFile::load(
    const godot::Ref<RiveResource>& p_resource
) {
    godot::Ref<RiveFile> file = memnew(RiveFile);
    file->m_resource = p_resource;

    auto bytes = file->m_resource->get_bytes();

    // TODO: inject correct renderer
//    static rive::NoOpFactory factory;
    auto factory = RiveRenderer::get_singleton()->_get_factory();
    rive::ImportResult result;
    file->m_file = rive::File::import(rive::Span(bytes.ptr(), bytes.size()), factory, &result);
    ERR_FAIL_COND_V_MSG(result != rive::ImportResult::success, {}, "failed to import rive file from resource");

    return file;
}

RiveFile::RiveFile() = default;
RiveFile::~RiveFile() = default;

uint RiveFile::get_artboard_count() const {
    ERR_FAIL_NULL_V_MSG(m_file, 0, "m_file is null");
    return m_file->artboardCount();
}

godot::String RiveFile::get_default_artboard_name() const {
    ERR_FAIL_NULL_V_MSG(m_file, "", "m_file is null");
    return {m_file->artboard()->name().c_str()};
}


godot::String RiveFile::get_artboard_name(uint p_index) const {
    ERR_FAIL_NULL_V_MSG(m_file, "", "m_file is null");
    return {m_file->artboardNameAt(p_index).c_str()};
}

godot::TypedArray<godot::String> RiveFile::get_artboard_names() const {
    godot::TypedArray<godot::String> names;

    for (size_t i = 0; i < m_file->artboardCount(); ++i) {
        names.append(get_artboard_name(i));
    }

    return names;
}

godot::Ref<RiveArtboard> RiveFile::instantiate_default_artboard() const {
    ERR_FAIL_NULL_V_MSG(m_file, {}, "m_file is null");
    ERR_FAIL_NULL_V_MSG(m_file->artboard(), {}, "file does not have default artboard");

    // Initialize the artboard object with an artboard instance.
    godot::Ref<RiveArtboard> artboard = memnew(RiveArtboard(m_file->artboardDefault()));

    return artboard;
}

godot::Ref<RiveArtboard> RiveFile::instantiate_artboard(uint p_index) const {
    ERR_FAIL_NULL_V_MSG(m_file, {}, "m_file is null");
    ERR_FAIL_NULL_V_MSG(m_file->artboard(p_index), {}, "artboard p_index does not exist");

    // Initialize the artboard object with an artboard instance.
    godot::Ref<RiveArtboard> artboard = memnew(RiveArtboard(m_file->artboardAt(p_index)));

    return artboard;
}

godot::Ref<RiveArtboard> RiveFile::instantiate_artboard_named(const godot::String& p_name) const {
    ERR_FAIL_NULL_V_MSG(m_file, {}, "m_file is null");

    auto name = std::string(p_name.utf8().ptr());
    ERR_FAIL_NULL_V_MSG(m_file->artboard(name), {}, "artboard p_index does not exist");

    // Initialize the artboard object with an artboard instance.
    godot::Ref<RiveArtboard> artboard = memnew(RiveArtboard(m_file->artboardNamed(name)));

    return artboard;
}






