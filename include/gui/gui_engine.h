#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "simdjson.h"
#include "backends/imgui_impl_vulkan.h"

// Data
static VkAllocationCallbacks*   g_Allocator = nullptr;
static VkInstance               g_Instance = VK_NULL_HANDLE;
static VkPhysicalDevice         g_PhysicalDevice = VK_NULL_HANDLE;
static VkDevice                 g_Device = VK_NULL_HANDLE;
static uint32_t                 g_QueueFamily = (uint32_t)-1;
static VkQueue                  g_Queue = VK_NULL_HANDLE;
static VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
static VkPipelineCache          g_PipelineCache = VK_NULL_HANDLE;
static VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;

static ImGui_ImplVulkanH_Window g_MainWindowData;
static int                      g_MinImageCount = 2;
static bool                     g_SwapChainRebuild = false;

// Font Data
static ImFont* Inter_Var_Font;

class GUIEngine {

    public:

    // Global functions
    static GUIEngine get();

    // What windows are showing
    bool show_another_window = true;

    ImGuiContext* context = nullptr;

    // Background Color
    ImVec4 background = ImVec4(102.0f/255.0f, 102.0f/255.0f, 102.0f/255.0f, 1.00f);
    ImGuiIO* io = nullptr;

    // Window
    GLFWwindow* window = nullptr;

    // ImGUI/Vulkan window
    ImGui_ImplVulkanH_Window* wd = nullptr;

    int init();
    static void glfw_error_callback(int error, const char* description);

    static void check_vk_result(VkResult err);

    bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);

    VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();

    void SetupVulkan(ImVector<const char*> instance_extensions);

    // All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers used by the demo.
        // Your real engine/app may not use them.
    void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);

    void CleanupVulkan();

    void CleanupVulkanWindow();

    void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);

    void FramePresent(ImGui_ImplVulkanH_Window* wd);
    void run();

    void GUI_Clean();
};
