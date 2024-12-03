include(ExternalProject)
include(FetchContent)

# Fetch the Rive repo with FetchContent so that we can
# evaluate the include directory at configure time.
#
# see: https://discourse.cmake.org/t/can-you-combine-fetchcontent-and-externalproject/228/2
FetchContent_Declare(rive-cpp-download
        # Fetch the latest sources from the Rive Github repository.
        GIT_REPOSITORY "https://github.com/rive-app/rive-cpp.git"
        GIT_TAG 4aeeaddf4aa621fdcdd48a85bb287d5c24ab9f27
)

# Fetch the Rive content if necessary.
FetchContent_GetProperties(rive-cpp-download)
if(NOT rive-cpp-download_POPULATED)
    FetchContent_Populate(rive-cpp-download)
endif()


# Create a library for building the Rive library sources.
#
# We where using ExternalProject_Add and running premake5 and make,
# but the resulting library was causing linking errors that where
# not straightforward to resolve due to the premake5 generated
# makefiles calling ar for linking instead of using the clang linker.
add_library(rive-cpp STATIC
        ${rive-cpp-download_SOURCE_DIR}/utils/no_op_factory.cpp

        # for i in **/*.cpp; do [ -f "$i" ] && echo "\${rive-cpp-download_SOURCE_DIR}/src/$i"; done
        ${rive-cpp-download_SOURCE_DIR}/src/animation/animation_state.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/animation_state_instance.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/blend_animation.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/blend_animation_1d.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/blend_animation_direct.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/blend_state.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/blend_state_1d.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/blend_state_1d_instance.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/blend_state_direct.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/blend_state_direct_instance.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/blend_state_transition.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/cubic_ease_interpolator.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/cubic_interpolator.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/cubic_interpolator_component.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/cubic_interpolator_solver.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/cubic_value_interpolator.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/elastic_ease.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/elastic_interpolator.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/interpolating_keyframe.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/keyed_object.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/keyed_property.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/keyframe.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/keyframe_bool.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/keyframe_callback.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/keyframe_color.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/keyframe_double.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/keyframe_id.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/keyframe_interpolator.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/keyframe_string.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/layer_state.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/linear_animation.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/linear_animation_instance.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/listener_action.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/listener_align_target.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/listener_bool_change.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/listener_fire_event.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/listener_input_change.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/listener_number_change.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/listener_trigger_change.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/nested_animation.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/nested_bool.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/nested_linear_animation.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/nested_number.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/nested_remap_animation.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/nested_simple_animation.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/nested_state_machine.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/nested_trigger.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/state_instance.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/state_machine.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/state_machine_input.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/state_machine_input_instance.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/state_machine_instance.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/state_machine_layer.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/state_machine_listener.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/state_transition.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/system_state_instance.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/transition_bool_condition.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/transition_condition.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/transition_number_condition.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/animation/transition_trigger_condition.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/artboard.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/assets/audio_asset.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/assets/file_asset.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/assets/file_asset_contents.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/assets/file_asset_referencer.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/assets/font_asset.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/assets/image_asset.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/audio/audio_engine.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/audio/audio_reader.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/audio/audio_sound.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/audio/audio_source.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/audio_event.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/bones/bone.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/bones/root_bone.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/bones/skin.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/bones/skinnable.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/bones/tendon.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/bones/weight.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/component.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/constraints/constraint.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/constraints/distance_constraint.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/constraints/follow_path_constraint.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/constraints/ik_constraint.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/constraints/rotation_constraint.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/constraints/scale_constraint.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/constraints/targeted_constraint.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/constraints/transform_constraint.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/constraints/translation_constraint.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/container_component.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/core/binary_reader.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/core/field_types/core_bool_type.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/core/field_types/core_bytes_type.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/core/field_types/core_color_type.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/core/field_types/core_double_type.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/core/field_types/core_string_type.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/core/field_types/core_uint_type.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/dependency_sorter.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/draw_rules.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/draw_target.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/drawable.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/event.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/factory.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/file.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/animation_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/animation_state_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/any_state_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/blend_animation_1d_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/blend_animation_direct_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/blend_state_1d_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/blend_state_direct_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/blend_state_transition_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/cubic_ease_interpolator_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/cubic_interpolator_component_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/cubic_value_interpolator_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/elastic_interpolator_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/entry_state_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/exit_state_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/keyed_object_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/keyed_property_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/keyframe_bool_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/keyframe_callback_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/keyframe_color_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/keyframe_double_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/keyframe_id_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/keyframe_string_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/linear_animation_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/listener_align_target_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/listener_bool_change_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/listener_fire_event_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/listener_number_change_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/listener_trigger_change_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/nested_bool_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/nested_number_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/nested_remap_animation_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/nested_simple_animation_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/nested_state_machine_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/nested_trigger_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/state_machine_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/state_machine_bool_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/state_machine_fire_event.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/state_machine_fire_event_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/state_machine_layer_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/state_machine_listener_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/state_machine_number_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/state_machine_trigger_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/state_transition_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/transition_bool_condition_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/transition_number_condition_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/animation/transition_trigger_condition_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/artboard_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/assets/audio_asset_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/assets/file_asset_contents_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/assets/folder_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/assets/font_asset_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/assets/image_asset_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/audio_event_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/backboard_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/bones/bone_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/bones/cubic_weight_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/bones/root_bone_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/bones/skin_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/bones/tendon_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/bones/weight_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/constraints/distance_constraint_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/constraints/follow_path_constraint_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/constraints/ik_constraint_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/constraints/rotation_constraint_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/constraints/scale_constraint_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/constraints/transform_constraint_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/constraints/translation_constraint_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/custom_property_boolean_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/custom_property_number_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/custom_property_string_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/draw_rules_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/draw_target_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/event_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/joystick_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/nested_artboard_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/node_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/open_url_event_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/clipping_shape_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/contour_mesh_vertex_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/cubic_asymmetric_vertex_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/cubic_detached_vertex_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/cubic_mirrored_vertex_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/ellipse_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/image_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/mesh_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/mesh_vertex_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/paint/fill_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/paint/gradient_stop_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/paint/linear_gradient_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/paint/radial_gradient_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/paint/solid_color_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/paint/stroke_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/paint/trim_path_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/points_path_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/polygon_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/rectangle_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/shape_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/star_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/straight_vertex_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/shapes/triangle_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/solo_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/text/text_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/text/text_modifier_group_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/text/text_modifier_range_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/text/text_style_axis_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/text/text_style_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/text/text_style_feature_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/text/text_value_run_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/generated/text/text_variation_modifier_base.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/hittest_command_path.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/importers/artboard_importer.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/importers/backboard_importer.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/importers/file_asset_importer.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/importers/keyed_object_importer.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/importers/keyed_property_importer.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/importers/layer_state_importer.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/importers/linear_animation_importer.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/importers/state_machine_importer.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/importers/state_machine_layer_component_importer.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/importers/state_machine_layer_importer.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/importers/state_machine_listener_importer.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/importers/state_transition_importer.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/joystick.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/layout.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/math/aabb.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/math/contour_measure.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/math/hit_test.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/math/mat2d.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/math/mat2d_find_max_scale.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/math/raw_path.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/math/raw_path_utils.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/math/vec2d.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/nested_artboard.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/node.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/renderer.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/scene.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/clipping_shape.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/cubic_asymmetric_vertex.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/cubic_detached_vertex.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/cubic_mirrored_vertex.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/cubic_vertex.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/ellipse.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/image.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/mesh.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/mesh_vertex.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/metrics_path.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/paint/color.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/paint/fill.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/paint/gradient_stop.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/paint/linear_gradient.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/paint/radial_gradient.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/paint/shape_paint.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/paint/shape_paint_mutator.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/paint/solid_color.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/paint/stroke.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/paint/trim_path.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/parametric_path.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/path.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/path_composer.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/path_vertex.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/points_path.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/polygon.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/rectangle.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/shape.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/shape_paint_container.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/star.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/straight_vertex.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/triangle.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/shapes/vertex.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/simple_array.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/solo.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/static_scene.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/text/font_hb.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/text/glyph_lookup.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/text/line_breaker.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/text/text.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/text/text_modifier.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/text/text_modifier_group.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/text/text_modifier_range.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/text/text_style.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/text/text_style_axis.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/text/text_style_feature.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/text/text_value_run.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/text/text_variation_modifier.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/text/utf.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/transform_component.cpp
        ${rive-cpp-download_SOURCE_DIR}/src/world_transform_component.cpp
)

target_compile_definitions(rive-cpp PUBLIC
        WITH_RIVE_TOOLS # include embedded asset tools
        WITH_RIVE_TEXT  # enable text rendering support

        WITH_RIVE_AUDIO # include audio support
        EXTERNAL_RIVE_AUDIO_ENGINE # we'll use an external (godot) audio engine
        MA_NO_DEVICE_IO # disable for external audio engine
        MA_NO_RESOURCE_MANAGER # disable for external audio engine
)

set_target_properties(rive-cpp PROPERTIES
        INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${rive-cpp-download_SOURCE_DIR}/include
)

target_include_directories(rive-cpp SYSTEM PUBLIC
        ${rive-cpp-download_SOURCE_DIR}/include
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/rive-cpp)

include( rive-miniaudio ) # just adds an include directory
include( rive-harfbuzz )
include( rive-sheenbidi )

target_include_directories(rive-cpp SYSTEM PUBLIC ${RIVE_MINIAUDIO_INCLUDE_DIRECTORY})
target_link_libraries(rive-cpp PUBLIC rive-harfbuzz)

target_link_libraries(rive-cpp PRIVATE rive-sheenbidi)

#target_link_libraries(rive-cpp PUBLIC debug rive-sheenbidi-debug)
#target_link_libraries(rive-cpp PUBLIC optimized rive-sheenbidi-release)

# Create a library for building the Rive library sources.
#
# We where using ExternalProject_Add and running premake5 and make,
# but the resulting library was causing linking errors that where
# not straightforward to resolve due to the premake5 generated
# makefiles calling ar for linking instead of using the clang linker.
add_library(rive-skia STATIC
        # for i in **/*.cpp; do [ -f "$i" ] && echo "\${rive-cpp-download_SOURCE_DIR}/skia/renderer/src/$i"; done
#        ${rive-cpp-download_SOURCE_DIR}/skia/renderer/src/cg_factory.cpp
        ${rive-cpp-download_SOURCE_DIR}/skia/renderer/src/cg_skia_factory.cpp
#        ${rive-cpp-download_SOURCE_DIR}/skia/renderer/src/mac_utils.cpp
        ${rive-cpp-download_SOURCE_DIR}/skia/renderer/src/skia_factory.cpp

        ${rive-cpp-download_SOURCE_DIR}/cg_renderer/src/cg_factory.cpp
)

add_dependencies(rive-skia rive-cpp unofficial::skia::skia)

target_include_directories(rive-skia INTERFACE
        ${rive-cpp-download_SOURCE_DIR}/skia/renderer/include
        ${rive-cpp-download_SOURCE_DIR}/cg_renderer/include)

target_include_directories(rive-skia PUBLIC
        ${rive-cpp-download_SOURCE_DIR}/skia/renderer/include
        ${rive-cpp-download_SOURCE_DIR}/cg_renderer/include)


find_package(unofficial-skia CONFIG REQUIRED)

target_link_libraries(rive-skia PUBLIC rive-cpp unofficial::skia::skia)