

#include <map>

#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/image_texture.hpp>

#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <vulkan/vulkan.h>

#include <include/gpu/GrDirectContext.h>
#include <include/gpu/vk/GrVkBackendContext.h>

#include <skia_renderer.hpp>

#include "render_target.h"

#include "runtime/rive_artboard.h"

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

void RiveRenderTargetSkiaVulkan::_bind_methods() {

}

RiveRenderTargetSkiaVulkan::RiveRenderTargetSkiaVulkan() = default;

RiveRenderTargetSkiaVulkan::RiveRenderTargetSkiaVulkan(godot::Point2 p_size) : RiveRenderTargetSkia(p_size) {
}

RiveRenderTargetSkiaVulkan::~RiveRenderTargetSkiaVulkan() {
    if (m_vk_device != VK_NULL_HANDLE) {
        vkDestroyDevice(m_vk_device, nullptr);
    }

    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(m_vk_instance, m_vk_debug_messenger, nullptr);
    }

    if (m_vk_instance != VK_NULL_HANDLE) {
        vkDestroyInstance(m_vk_instance, nullptr);
    }

    if(m_window != nullptr) {
        glfwDestroyWindow(m_window);
    }


    //    glfwTerminate();
}



void RiveRenderTargetSkiaVulkan::_create_renderer() {
    ERR_FAIL_COND_MSG(m_size.is_zero_approx(), "Cannot initialize render target with zero size.");

    // Rive works in floats for sizes, so we need to round to whole numbers for GPU image sizing.
    auto width = static_cast<int>(get_width());
    auto height = static_cast<int>(get_height());

    // (Re)initialize the texture image.
    auto base_image = godot::Image::create(width, height, false, godot::Image::Format::FORMAT_RGBA8);
    base_image->fill(godot::Color(0,0,0)); // fill with black so that we can see the control when there is nothing rendered.
    m_texture->set_image(base_image);

    // Resize the frame data buffer to fit the image.
    m_frame_data.resize(width * height * 4);

    // Wipe the frame data buffer.
    memset(m_frame_data.ptrw(), 0, m_frame_data.size());

    // NOTE: The image type used here is important! It MUST match the layout of the image
    //  type used for the Godot image backing the render target in Godot's image space.
    m_info = SkImageInfo::Make(width, height,
                                  SkColorType::kRGBA_8888_SkColorType,
                                  SkAlphaType::kPremul_SkAlphaType);

    // Create a new surface for the new size.
    godot::UtilityFunctions::print(godot::vformat("creating SkSurface for %dx%d texture rid=%d", m_size.width, m_size.height, m_texture->get_rid().get_id()));
//    m_surface = SkSurface::MakeRasterDirect(m_info, m_frame_data.ptrw(), m_info.minRowBytes(), nullptr);
    m_surface = SkSurface::MakeRenderTarget(m_context.get(), SkBudgeted::kNo, m_info);
    ERR_FAIL_NULL_MSG(m_surface, "Failed to create Skia GPU surface.");

    // Get the derived canvas and create a renderer from it.
    m_canvas = m_surface->getCanvas();
    m_renderer = std::make_unique<rive::SkiaRenderer>(m_canvas);
}

void glfw_vulkan_error_callback(int code, const char* description) {
    ERR_PRINT(godot::vformat("A GLFW error occurred code=%s: %s", code, description));
}


std::vector<const char*> get_required_extensions(bool enableValidationLayers) {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions;
    for(uint32_t i = 0; i < glfwExtensionCount; i++) {
        extensions.emplace_back(glfwExtensions[i]);
    }

    // Required on macOS with the latest MoltenVK sdk to prevent VK_ERROR_INCOMPATIBLE_DRIVER
    // due to the VK_KHR_PORTABILITY_subset extension being mandatory.
    // ref: https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Instance
    extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME); // VK_KHR_portability_subset

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}
godot::Error RiveRenderTargetSkiaVulkan::create_instance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Rive Skia Renderer";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_1;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = get_required_extensions(enableValidationLayers);

    // Required on macOS with the latest MoltenVK sdk to prevent VK_ERROR_INCOMPATIBLE_DRIVER
    // due to the VK_KHR_PORTABILITY_subset extension being mandatory.
    // ref: https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Instance
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    ERR_FAIL_COND_V_MSG(enableValidationLayers && !check_validation_layer_support(), godot::FAILED, "Validation layers requested, but not available!");

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populate_debug_messenger_create_info(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    VkResult result = vkCreateInstance(&createInfo, nullptr, &m_vk_instance);
    ERR_FAIL_COND_V_MSG(result != VK_SUCCESS, godot::FAILED, "Failed to create Vulkan instance.");

    // Debug logging of available Vulkan extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> available_extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, available_extensions.data());
    std::cout << "available extensions:\n";
    for (const auto& extension : available_extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }

    return godot::OK;
}

bool RiveRenderTargetSkiaVulkan::check_validation_layer_support() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

void RiveRenderTargetSkiaVulkan::populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = vk_debug_callback;
//    debugCreateInfo.pUserData = nullptr; // Optional
}

godot::Error RiveRenderTargetSkiaVulkan::setup_debug_messenger() {
    if (!enableValidationLayers) return godot::OK;

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    populate_debug_messenger_create_info(debugCreateInfo);

    ERR_FAIL_COND_V_MSG(CreateDebugUtilsMessengerEXT(m_vk_instance, &debugCreateInfo, nullptr, &m_vk_debug_messenger) != VK_SUCCESS, godot::FAILED, "Failed to setup debug messenger.");

    return godot::OK;
}

//bool RiveRenderTargetSkiaVulkan::is_device_suitable(VkPhysicalDevice device) {
//    VkPhysicalDeviceProperties deviceProperties;
//    VkPhysicalDeviceFeatures deviceFeatures;
//    vkGetPhysicalDeviceProperties(device, &deviceProperties);
//    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
//
//    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
//           deviceFeatures.geometryShader;
//}

int RiveRenderTargetSkiaVulkan::rate_device_suitability(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    int score = 0;

    // We need to have a valid queue family.
    QueueFamilyIndices indices = find_queue_families(device);
    if(!indices.is_complete()) {
        return 0;
    }

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
        score += 500;
    } else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        // Discrete GPUs have a significant performance advantage
        score += 1000;
    }

    // Maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;

    // Application can't function without geometry shaders
//    if (!deviceFeatures.geometryShader) {
//        return 0;
//    }

    return score;
}

godot::Error RiveRenderTargetSkiaVulkan::pick_physical_device() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_vk_instance, &deviceCount, nullptr);
    ERR_FAIL_COND_V_MSG(deviceCount == 0, godot::FAILED, "Failed to find GPU with Vulkan support!");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_vk_instance, &deviceCount, devices.data());

//    for (const auto& device : devices) {
//        if (is_device_suitable(device)) {
//            m_vk_physical_device = device;
//            break;
//        }
//    }
//    ERR_FAIL_COND_V_MSG(m_vk_physical_device == VK_NULL_HANDLE, godot::FAILED, "Failed to find suitable GPU!");

    // Use an ordered map to automatically sort candidates by increasing score
    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto& device : devices) {
        int score = rate_device_suitability(device);
        candidates.insert(std::make_pair(score, device));
    }

    // Check if the best candidate is suitable at all
    if (candidates.rbegin()->first > 0) {
        m_vk_physical_device = candidates.rbegin()->second;
    } else {
        ERR_PRINT("Failed to find suitable GPU!");
        return godot::FAILED;
    }

    return godot::OK;
}

RiveRenderTargetSkiaVulkan::QueueFamilyIndices RiveRenderTargetSkiaVulkan::find_queue_families(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        if (indices.is_complete()) {
            break;
        }

        i++;
    }

    return indices;
}

godot::Error RiveRenderTargetSkiaVulkan::create_logical_device() {
    m_indices = find_queue_families(m_vk_physical_device);

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = m_indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    // Vulkan requires a queue priority even for a single queue.
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    createInfo.pEnabledFeatures = &deviceFeatures;

    std::vector<const char*> device_extensions;
    device_extensions.emplace_back("VK_KHR_portability_subset");

    // TODO: Added because of error "VK_KHR_portability_subset must be enabled because physical device VkPhysicalDevice 0x600001534000[] supports it. The Vulkan spec states: If the VK_KHR_portability_subset extension is included in pProperties of vkEnumerateDeviceExtensionProperties, ppEnabledExtensionNames must include "VK_KHR_portability_subset"
    createInfo.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
    createInfo.ppEnabledExtensionNames = device_extensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    ERR_FAIL_COND_V_MSG(vkCreateDevice(m_vk_physical_device, &createInfo, nullptr, &m_vk_device) != VK_SUCCESS, godot::FAILED, "Failed to create logical device!");

    vkGetDeviceQueue(m_vk_device, m_indices.graphicsFamily.value(), 0, &m_vk_graphics_queue);

    return godot::OK;
}

godot::Error RiveRenderTargetSkiaVulkan::_initialize() {

#ifdef __APPLE__
    // Hides the main menu and dock icon on macOS for our headless window(s).
    glfwInitHint(GLFW_COCOA_MENUBAR, GLFW_FALSE);
#endif
    if (!glfwInit()) {
        ERR_PRINT("Failed to initialize GLFW library for the Rive Renderer's Vulkan-based Skia backend.");
        return godot::FAILED;
    }

    // TODO: don't think we even need to GLFW window with Vulkan since
    //   Vulkan is headless and doesn't require a window by default.

    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    godot::UtilityFunctions::print(godot::vformat("Using GLFW %d.%d.%d", major, minor, revision));
    godot::UtilityFunctions::print(glfwGetVersionString());

    glfwSetErrorCallback(glfw_vulkan_error_callback);

    ERR_FAIL_COND_V_MSG(glfwVulkanSupported() != GLFW_TRUE, godot::FAILED, "Vulkan support not found.");

    // Instruct GLFW NOT to create an OpenGL context.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Create an invisible window for offscreen rendering.
    godot::UtilityFunctions::print("Creating off-screen GLFW context for Rive's Skia renderer");
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    m_window = glfwCreateWindow(static_cast<int>(get_width()), static_cast<int>(get_height()), "Rive OpenGL Renderer", NULL, NULL);
    ERR_FAIL_NULL_V_MSG(m_window, godot::FAILED, "Failed to create offscreen GLFW window for Skia.");

    // TODO: Use of GrGLMakeNativeInterface() SHOULD support iOS, Mac, Android, WebGL, etc. but will need to be tested.
    godot::UtilityFunctions::print("Creating Skia rendering context for Vulkan");

    // Create the instance, physical and logical devices, and the queue.
    ERR_FAIL_COND_V_MSG(create_instance() != godot::OK, godot::FAILED, "Failed to create VkInstance!");
    ERR_FAIL_COND_V_MSG(setup_debug_messenger() != godot::OK, godot::FAILED, "Failed to setup debug listened!");
    ERR_FAIL_COND_V_MSG(pick_physical_device() != godot::OK, godot::FAILED, "Failed to pick physical device!");
    ERR_FAIL_COND_V_MSG(create_logical_device() != godot::OK, godot::FAILED, "Failed to create logical device!");

    // Construct the Skia Vulkan backend parameters.
    GrVkBackendContext vk_backend_context;
    vk_backend_context.fInstance = m_vk_instance;
    vk_backend_context.fPhysicalDevice = m_vk_physical_device;
    vk_backend_context.fDevice = m_vk_device;
    vk_backend_context.fQueue = m_vk_graphics_queue;
    vk_backend_context.fGetProc = std::function([=](const char* name, VkInstance instance, VkDevice dev) -> PFN_vkVoidFunction {
        PFN_vkVoidFunction ptr = nullptr;
        if (instance == VK_NULL_HANDLE && dev == VK_NULL_HANDLE) {
            ptr = vkGetInstanceProcAddr(VK_NULL_HANDLE, name);
        } else if (instance == VK_NULL_HANDLE && dev != VK_NULL_HANDLE) {
            ptr = vkGetDeviceProcAddr(dev, name);
        } else if (instance != VK_NULL_HANDLE && dev == VK_NULL_HANDLE) {
            ptr = vkGetInstanceProcAddr(instance, name);
        } else {
            ptr = vkGetDeviceProcAddr(dev, name);
        }
        if (!ptr)
            ERR_PRINT(godot::vformat("The extension [%s] required by Skia is not satisfied!", name));
        return ptr;
    });

    m_context = GrDirectContext::MakeVulkan(vk_backend_context);

    _create_renderer();

    return godot::OK;
}

void RiveRenderTargetSkiaVulkan::resize(godot::Point2 p_size) {
    ERR_FAIL_COND_MSG(p_size.is_zero_approx(), "Zero size passed to RiveRenderTargetSkiaVulkan::resize.");

    if (m_size == p_size) {
        godot::UtilityFunctions::print("RiveRenderTargetSkiaVulkan::resize skipped, matches old size");
        return;
    }

    godot::UtilityFunctions::print(godot::vformat("RiveRenderTargetSkiaVulkan::resize %dx%d", p_size.width, p_size.height));

    m_size = p_size;

    // For this CPU raster backend, we re-create the SkSurface, SkCanvas, and Rive renderer every time the target size
    // changes. This is because with Raster rendering we don't have any GPU-backed Skia resources we can dynamically
    // resize, so we just need to recreate the entire surface.
    _create_renderer();
}

void RiveRenderTargetSkiaVulkan::_update() {
    // Only applicable to rendering device based targets
//    if (godot::OS::get_singleton()->get_thread_caller_id() != godot::OS::get_singleton()->get_main_thread_id()) {
//        WARN_PRINT("Calling RiveRenderTargetSkiaVulkan::_update from a non-main or non-renderserver thread may stall the RenderingServer on texture updates.");
//    }

    // TODO: thread drawing/updating, but ensure texture is only updated from main thread. call_on_render_thread?

    ERR_FAIL_NULL_MSG(m_surface, "Cannot update from a null SkiSurface.");
    ERR_FAIL_COND_MSG(m_surface->width() != static_cast<int>(get_width()), "SkSurface width does not match render target width.");
    ERR_FAIL_COND_MSG(m_surface->height() != static_cast<int>(get_height()), "SkSurface height does not match render target height.");

    // Read the pixels from the GPU-backed surface directly into our frame buffer.
    auto successful_read = m_surface->readPixels(m_info, m_frame_data.ptrw(), m_info.minRowBytes(), 0, 0);
    ERR_FAIL_COND_MSG(!successful_read, "Cannot read pixels from SkSurface!");

    // Create the new image from the frame data and update it in Godot's GPU context.
    godot::Ref<godot::Image> frame_image = godot::Image::create_from_data(
            get_width(), get_height(), false,
            godot::Image::FORMAT_RGBA8, m_frame_data); // TODO: zero copy image creation? https://github.com/godotengine/godot/blob/2a0aef5f0912b60f85c9e150cc0bfbeab7de6e40/modules/theora/video_stream_theora.cpp#LL116C19-L116C25
    m_texture->update(frame_image);
}

void RiveRenderTargetSkiaVulkan::_draw_begin(const rive::AABB& content_bounds) {
    // Perform a "clear" on the image data.
    //
    // NOTE: clearing the canvas pixel buffer with memset is faster than using clear/drawColor.
    memset(m_frame_data.ptrw(), 0, m_frame_data.size());

    RiveRenderTargetSkia::_draw_begin(content_bounds);
}

void RiveRenderTargetSkiaVulkan::_draw_end() {
    RiveRenderTargetSkia::_draw_end();

    // Update the texture data.
    _update();
}

void RiveRenderTargetSkiaVulkan::draw_artboard(const godot::Ref<RiveArtboard> &p_artboard) {
    ERR_FAIL_COND_MSG(!p_artboard.is_valid(), "Cannot render null RiveArtboard.");

    // Clear the frame image and set renderer transform and alignment.
    _draw_begin(p_artboard->_get_rive_bounds());

    // Render the object.
    p_artboard->_get_rive_artboard_instance()->draw(m_renderer.get());

    // Update the target texture with the rendered frame.
    _draw_end();
}

void RiveRenderTargetSkiaVulkan::draw_animation(const godot::Ref<RiveAnimation> &p_animation) {
    ERR_FAIL_COND_MSG(!p_animation.is_valid(), "Cannot render null RiveAnimation.");

    // Clear the frame image and set renderer transform and alignment.
    _draw_begin(p_animation->_get_rive_bounds());

    // Render the object.
    p_animation->_get_rive_animation_instance()->draw(m_renderer.get());

    // Update the target texture with the rendered frame.
    _draw_end();
}

void RiveRenderTargetSkiaVulkan::draw_state_machine(const godot::Ref<RiveStateMachine> &p_state_machine) {
    ERR_FAIL_COND_MSG(!p_state_machine.is_valid(), "Cannot render null RiveStateMachine.");

    // Clear the frame image and set renderer transform and alignment.
    _draw_begin(p_state_machine->_get_rive_bounds());

    // Render the object.
    p_state_machine->_get_rive_state_machine_instance()->draw(m_renderer.get());

    // Update the target texture with the rendered frame.
    _draw_end();
}







