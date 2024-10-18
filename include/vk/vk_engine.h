#pragma once

#include "vk/vk_types.h"
#include <vulkan/vulkan_core.h>

struct FrameData {
    VkCommandPool _commandPool;
    VkCommandBuffer _mainCommandBuffer;

    VkSemaphore _swapchainSemaphore, _renderSemaphore;
    VkFence _renderFence;
};

// Double-Buffering
constexpr unsigned int FRAME_OVERLAP = 2;

class VulkanEngine {
public:
    // Frame Data variables
    FrameData _frames[FRAME_OVERLAP];
    FrameData& get_current_frame() { return _frames[_frameNumber % FRAME_OVERLAP]; }

    // Command Queue
    VkQueue _graphicsQueue;
    uint32_t _graphicsQueueFamily;

    // Window variables
    bool _isInitialized {false};
    int _frameNumber {0};
    bool stop_rendering {false};

    int WIDTH {1440};
    int HEIGHT {810};
    VkExtent2D _windowsExtent;
    struct GLFWwindow* _window {nullptr};

    // Vulkan Variables
    VkInstance _instance;
    VkDebugUtilsMessengerEXT _debug_messenger;
    VkPhysicalDevice _chosenGPU;
    VkDevice _device;
    VkSurfaceKHR _surface;

    // VK Swapchain
    VkSwapchainKHR _swapchain;
    VkFormat _swapchainImageFormat;

    std::vector<VkImage> _swapchainImages;
    std::vector<VkImageView> _swapchainImageViews;
    VkExtent2D _swapchainExtent;

    // Global Functions
    static VulkanEngine& Get();
    void init();
    void cleanup();
    void draw();
    void run();

private:
    // GLFW Callback functions
    static void window_iconify_callback(GLFWwindow* window, int iconified);

    // Vulkan Inits
    void init_vulkan();
    void init_swapchain();
    void init_commands();
    void init_sync_structures();

    // Swapchain
    void create_swapchain(uint32_t width, uint32_t height);
    void destroy_swapchain();
};
