#include "vk/vk_engine.h"
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vk/vk_initializers.h"
#include "VkBootstrap.h"

#include <chrono>
#include <thread>

constexpr bool bUseValidationLayers = false;

VulkanEngine* loadedEngine = nullptr;

VulkanEngine& VulkanEngine::Get() { return *loadedEngine; }

// GLFW Event Handling Start
void VulkanEngine::window_iconify_callback(GLFWwindow* window, 
        int iconified) {
    VulkanEngine::Get().stop_rendering = (bool) iconified;
    fmt::println("stop_rendering = {}", VulkanEngine::Get().stop_rendering);
}
// GLFW Event Handling End

// Initialization functions
void VulkanEngine::init_vulkan() {
    vkb::InstanceBuilder builder;

    auto inst_ret = builder.set_app_name("TBDEngine")
        .request_validation_layers(bUseValidationLayers)
        .use_default_debug_messenger()
        .require_api_version(1, 3, 0)
        .build();
    
    vkb::Instance vkb_inst = inst_ret.value();

    _instance = vkb_inst.instance;
    _debug_messenger = vkb_inst.debug_messenger;
    
    // Create Vulkan surface using GLFW window
    if (glfwCreateWindowSurface(_instance, _window, nullptr, &_surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create Vulkan surface!");
    }

    //vulkan 1.3 features
    VkPhysicalDeviceVulkan13Features features13 {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
    features13.dynamicRendering = true;
    features13.synchronization2 = true;
    
    //vulkan 1.2 features
	VkPhysicalDeviceVulkan12Features features12 { 
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
	features12.bufferDeviceAddress = true;
	features12.descriptorIndexing = true;

    // Choosing gpu to write surface into GLFW
    vkb::PhysicalDeviceSelector selector {vkb_inst};
    vkb::PhysicalDevice physicalDevice = selector
        .set_minimum_version(1, 3)
        .set_required_features_13(features13)
        .set_required_features_12(features12)
        .set_surface(_surface)
        .select()
        .value();

    // Create the final vulkan device
    vkb::DeviceBuilder deviceBuilder {physicalDevice};
    vkb::Device vkbDevice = deviceBuilder.build().value();

    // Get the GkDevice handle used in the rest of the vulkan app
    _device = vkbDevice.device;
    _chosenGPU = physicalDevice.physical_device;

    // Getting Vulkan Device
    _graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
    _graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();
}

// Vulkan Swapchain
void VulkanEngine::create_swapchain(uint32_t width, uint32_t height) {
    vkb::SwapchainBuilder swapchainBuilder {_chosenGPU, _device, _surface};

    _swapchainImageFormat = VK_FORMAT_B8G8R8_UNORM;

    vkb::Swapchain vkbSwapchain = swapchainBuilder
        //.use_default_format_seelction()
        .set_desired_format(VkSurfaceFormatKHR
            {.format = _swapchainImageFormat,.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR})
        // V-SYNC
        .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
        .set_desired_extent(width, height)
        .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
        .build()
        .value();

    _swapchainExtent = vkbSwapchain.extent;
    // store swapchain and its related images
    _swapchain = vkbSwapchain.swapchain;
    _swapchainImages = vkbSwapchain.get_images().value();
    _swapchainImageViews = vkbSwapchain.get_image_views().value();
}

void VulkanEngine::destroy_swapchain() {
    vkDestroySwapchainKHR(_device, _swapchain, nullptr);

    // destroy swapchain resources
    for (int i = 0; _swapchainImageViews.size(); i++) {
        vkDestroyImageView(_device, _swapchainImageViews[i], nullptr);
    }
}


void VulkanEngine::init_swapchain() {
    create_swapchain(_windowsExtent.width, _windowsExtent.height);
}

void VulkanEngine::init_commands() {
    // Create Command pool for commands submitted to the queue
    VkCommandPoolCreateInfo commandPoolInfo = {};
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.pNext = nullptr;
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolInfo.queueFamilyIndex = _graphicsQueueFamily;

    for (int i = 0; i < FRAME_OVERLAP; i++) {
        VK_CHECK(vkCreateCommandPool(_device, &commandPoolInfo,
            nullptr, &_frames[i]._commandPool));

        // Allocate the default command buffer
        VkCommandBufferAllocateInfo cmdAllocInfo =
            vkinit::command_buffer_allocate_info(_frames[i]._commandPool, 1);

        VK_CHECK(vkAllocateCommandBuffers(_device, &cmdAllocInfo,
            &_frames[i]._mainCommandBuffer));
    }
}

void VulkanEngine::init_sync_structures() {

}

void VulkanEngine::init() {
    assert(loadedEngine == nullptr);
    loadedEngine = this;

    _windowsExtent.width = WIDTH;
    _windowsExtent.height = HEIGHT;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    // TODO Have to set up event handling

    _window = glfwCreateWindow(
        WIDTH, 
        HEIGHT, 
        "HelloTriangle", 
        nullptr, 
        nullptr);

    glfwSetWindowIconifyCallback(_window, VulkanEngine::window_iconify_callback);
	glfwMakeContextCurrent(_window);

    init_vulkan();
    init_swapchain();
    init_commands();
    init_sync_structures();
    
    _isInitialized = true;
}

void VulkanEngine::cleanup() {
    if (_isInitialized) {
        // Make sure the GPU has finished its execution
        vkDeviceWaitIdle(_device);

        for (int i = 0; i < FRAME_OVERLAP; i++) {
            vkDestroyCommandPool(_device, _frames[i]._commandPool, nullptr);
        }

        destroy_swapchain();

        vkDestroySurfaceKHR(_instance, _surface, nullptr);
        vkDestroyDevice(_device, nullptr);

        vkb::destroy_debug_utils_messenger(_instance, _debug_messenger);

        glfwDestroyWindow(_window);
        glfwTerminate();
    }

    loadedEngine = nullptr;
}

void VulkanEngine::draw() {

}

void VulkanEngine::run() {
    while (glfwWindowShouldClose(_window) == 0) {
        if (stop_rendering) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
		glfwPollEvents();
    }
}
