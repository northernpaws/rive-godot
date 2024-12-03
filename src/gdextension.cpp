#include "gdextension_interface.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/editor_plugin_registration.hpp>

#include <gdextension_interface.h>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/rendering_server.hpp>

#include "editor/plugin/editor_plugin.h"
#include "editor/plugin/inspector/resource/resource_inspector_preview_control.h"
#include "runtime/rive_file.h"
#include "runtime/rive_artboard.h"
#include "runtime/rive_renderer.h"

#include "scene/rive_control.h"
#include "runtime/renderers/skia/rive_renderer_skia.h"
#include "runtime/renderers/skia/rive_render_target_skia.h"
#include "resources/rive_drawable.h"
#include "editor/plugin/inspector/drawable/rive_drawable_property_editor_control.h"

#ifdef __APPLE__
#include "runtime/renderers/skia/metal/render_target.h"
#include "runtime/renderers/skia/opengl/render_target.h"
#include "runtime/renderers/skia/vulkan/render_target.h"

#endif

/// @file
/// Register our classes with Godot.
namespace {
    godot::Ref<RiveResourceLoader> resource_loader;

    /// @brief Called by Godot to let us register our classes with Godot.
    ///
    /// @param p_level the level being initialized by Godot
    ///
    /// @see GDExtensionInit
    void initialize_extension( godot::ModuleInitializationLevel p_level ) {
        godot::UtilityFunctions::print("initialize_extension");
        if (p_level == godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
            // Resources
            godot::GDREGISTER_CLASS(RiveResource);
            godot::GDREGISTER_CLASS(RiveDrawable);

            // Objects
            godot::GDREGISTER_CLASS(RiveFile);
            godot::GDREGISTER_CLASS(RiveArtboard);

            godot::GDREGISTER_CLASS(RiveTexture);

            godot::GDREGISTER_CLASS(RiveRenderer);
            godot::GDREGISTER_CLASS(RiveRenderTarget);

            godot::GDREGISTER_CLASS(RiveRendererSkia);
            godot::GDREGISTER_CLASS(RiveRenderTargetSkia);

            godot::GDREGISTER_CLASS(RiveRenderTargetSkiaOpenGL);
            godot::GDREGISTER_CLASS(RiveRenderTargetSkiaVulkan);
            #ifdef __APPLE__
            godot::GDREGISTER_CLASS(RiveRenderTargetSkiaMetal);
            #endif
//
//            // Scene nodes
            godot::GDREGISTER_CLASS(RiveControl);

            // Register the resource loader for Rive graphics (.riv files).
            godot::GDREGISTER_CLASS(RiveResourceLoader);
            resource_loader.instantiate();
            godot::ResourceLoader::get_singleton()->add_resource_format_loader(resource_loader);
        }

        if (p_level == godot::MODULE_INITIALIZATION_LEVEL_EDITOR) {
            // Classes registered with the editor in RiveEditorPlugin.
            godot::GDREGISTER_INTERNAL_CLASS(RiveEditorMainScreen);

            godot::GDREGISTER_INTERNAL_CLASS(RiveResourceEditorInspectorPlugin);
            godot::GDREGISTER_INTERNAL_CLASS(RiveResourceInspectorPreviewControl);

            godot::GDREGISTER_INTERNAL_CLASS(RiveDrawableEditorInspectorPlugin);
            godot::GDREGISTER_INTERNAL_CLASS(EditorResourcePickerRiveResource);
            godot::GDREGISTER_INTERNAL_CLASS(RiveDrawablePickerDialog);
            godot::GDREGISTER_INTERNAL_CLASS(RiveDrawablePropertyEditorControl);

            godot::GDREGISTER_INTERNAL_CLASS(RiveEditorResourcePreviewGenerator);
            godot::GDREGISTER_INTERNAL_CLASS(RiveEditorResourceTooltipPlugin);

            godot::GDREGISTER_INTERNAL_CLASS(RiveEditorImportPlugin);

            godot::GDREGISTER_INTERNAL_CLASS(RiveEditorPlugin);
            godot::EditorPlugins::add_by_type<RiveEditorPlugin>();
        }

    }

    /// @brief Called by Godot to let us do any cleanup.
    ///
    /// @see GDExtensionInit
    void uninitialize_extension( godot::ModuleInitializationLevel p_level ) {
        godot::UtilityFunctions::print("uninitialize_extension");
        if (p_level == godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
            godot::ResourceLoader::get_singleton()->remove_resource_format_loader(resource_loader);
            resource_loader.unref();
        }

        if (p_level == godot::MODULE_INITIALIZATION_LEVEL_EDITOR) {
            godot::EditorPlugins::remove_by_type<RiveEditorPlugin>();
        }
    }
}

extern "C" {
    /// @brief This is the entry point for the shared library.
    ///
    /// @note The name of this function must match the "entry_symbol" in
    /// templates/template.*.gdextension.in
    ///
    /// @param p_get_proc_address the interface (need more info)
    /// @param p_library the library (need more info)
    /// @param r_initialization the intialization (need more info)
    ///
    /// @returns GDExtensionBool
    GDExtensionBool GDE_EXPORT rive_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
        godot::GDExtensionBinding::InitObject init_obj( p_get_proc_address, p_library, r_initialization );

        init_obj.register_initializer(initialize_extension);
        init_obj.register_terminator(uninitialize_extension);
        init_obj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}