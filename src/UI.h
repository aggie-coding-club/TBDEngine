
#pragma once
#include <GLFW/glfw3.h>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_vulkan.h"

class UI{

    public:

    // Global functions
    static UI get();
    // Data
    static VkAllocationCallbacks*   g_Allocator;
    static VkInstance               g_Instance;
    static VkPhysicalDevice         g_PhysicalDevice;
    static VkDevice                 g_Device;
    static uint32_t                 g_QueueFamily;
    static VkQueue                  g_Queue;
    static VkDebugReportCallbackEXT g_DebugReport;
    static VkPipelineCache          g_PipelineCache;
    static VkDescriptorPool         g_DescriptorPool;

    static ImGui_ImplVulkanH_Window g_MainWindowData;
    static int                      g_MinImageCount;
    static bool                     g_SwapChainRebuild;

    // Font Data
    static ImFont* Inter_Var_Font;
    static ImFont* Inter_Var_Italic_Font;

    // What windows are showing
    bool show_another_window = true;

    // Background Color
    ImVec4 background = ImVec4(102.0f/255.0f, 102.0f/255.0f, 102.0f/255.0f, 1.00f);

    // Window
    GLFWwindow* window = nullptr;

    // IO
    ImGuiIO& io = ImGui::GetIO();;

    // ImGUI/Vulkan window
    ImGui_ImplVulkanH_Window* wd = nullptr;

    int init();
    static void glfw_error_callback(int error, const char* description);

    static void check_vk_result(VkResult err);

    static bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);

    static VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();

    static void SetupVulkan(ImVector<const char*> instance_extensions);

    // All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers used by the demo.
        // Your real engine/app may not use them.
    static void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);

    static void CleanupVulkan();

    static void CleanupVulkanWindow();

    static void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);

    static void FramePresent(ImGui_ImplVulkanH_Window* wd);
    void run();

    void UI_Clean() const;
};
