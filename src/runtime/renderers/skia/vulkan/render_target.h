

#ifndef RIVE_GODOT_RENDER_TARGET_VULKAN_H
#define RIVE_GODOT_RENDER_TARGET_VULKAN_H

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include <rive/renderer.hpp>

#include <include/core/SkSurface.h>
#include <include/core/SkCanvas.h>

#include "runtime/rive_render_target.h"
#include "runtime/renderers/skia/rive_render_target_skia.h"

class RiveRenderTargetSkiaVulkan : public RiveRenderTargetSkia {
    GDCLASS(RiveRenderTargetSkiaVulkan, RiveRenderTargetSkia)

    // the last rendered frame
//    godot::PackedByteArray m_frame_data;

    GLFWwindow* m_window = nullptr;

    VkInstance m_vk_instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_vk_debug_messenger = VK_NULL_HANDLE;
    VkPhysicalDevice m_vk_physical_device = VK_NULL_HANDLE;
    VkDevice m_vk_device = VK_NULL_HANDLE;
    VkQueue m_vk_graphics_queue = VK_NULL_HANDLE;

    SkImageInfo m_info;

    sk_sp<GrDirectContext> m_context;

    /// in-memory image to be updated frame-by-frame.
    // TODO: circular image buffer?
    // TODO: write directly to texture image?
    godot::PackedByteArray m_frame_data;
//    godot::Ref<godot::Image> m_image;

    sk_sp<SkSurface> m_surface;

    const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif


    static VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData) {

        if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
            ERR_PRINT(godot::vformat("validation layer: %s", pCallbackData->pMessage));
        } else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            WARN_PRINT(godot::vformat("validation layer: %s", pCallbackData->pMessage));
        } else {
            godot::UtilityFunctions::print(godot::vformat("validation layer: %s", pCallbackData->pMessage));
        }

        return VK_FALSE;
    }


    godot::Error create_instance();
    bool check_validation_layer_support();
    void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    godot::Error setup_debug_messenger();

//    bool is_device_suitable(VkPhysicalDevice device);
    int rate_device_suitability(VkPhysicalDevice device);
    godot::Error pick_physical_device();

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;

        bool is_complete() {
            return graphicsFamily.has_value();
        }
    };

    QueueFamilyIndices m_indices;

    QueueFamilyIndices find_queue_families(VkPhysicalDevice device);
    godot::Error create_logical_device();
protected:
    static void _bind_methods();

    RiveRenderTargetSkiaVulkan();
    ~RiveRenderTargetSkiaVulkan() override;

    void _create_renderer();

    void _update();

    void _draw_begin(const rive::AABB& content_bounds) override;
    void _draw_end() override;
public:
    explicit RiveRenderTargetSkiaVulkan(godot::Point2 p_size);

    godot::Error _initialize() override;

    void resize(godot::Point2 p_size) override;

    void draw_artboard(const godot::Ref<RiveArtboard> &p_artboard) override;
    void draw_animation(const godot::Ref<RiveAnimation> &p_animation) override;
    void draw_state_machine(const godot::Ref<RiveStateMachine> &p_state_machine) override;


};


#endif //RIVE_GODOT_RENDER_TARGET_H
