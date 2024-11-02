// Code for File Hierarchy

#include "imgui.h"
#include "gui/Custom_Widgets.h"
void ShowFileHierarchy()
{
    // Remove Decorations for the window
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_AlwaysVerticalScrollbar;

    ImVec2 DisplaySize = ImGui::GetIO().DisplaySize;

    float window_width = DisplaySize.x / 4;
    float window_height = DisplaySize.y / 2.5;
    ImVec2 position(DisplaySize.x - window_width,29);

    // Set Window width
    ImGui::SetNextWindowSize(ImVec2(window_width,window_height));
    // Set window's position
    ImGui::SetNextWindowPos(position);

    // Create the window
    ImGui::Begin("Hierarchy",nullptr, window_flags);
    if (ImGui::TreeNode("Root Folder"))
    {
        // Folder 1
        if (ImGui::TreeNode("Folder 1"))
        {
            ImGui::Selectable("File 1.1");
            ImGui::Selectable("File 1.2");

            if (ImGui::TreeNode("Subfolder 1.1"))
            {
                ImGui::Selectable("File 1.1.1");
                ImGui::Selectable("File 1.1.2");
                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        // Folder 2
        if (ImGui::TreeNode("Folder 2"))
        {
            ImGui::Selectable("File 2.1");
            ImGui::Selectable("File 2.2");
            ImGui::TreePop();
        }

        // Folder 3
        if (ImGui::TreeNode("Folder 3"))
        {
            if (ImGui::TreeNode("Subfolder 3.1"))
            {
                ImGui::Selectable("File 3.1.1");
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Subfolder 3.2"))
            {
                ImGui::Selectable("File 3.2.1");
                ImGui::Selectable("File 3.2.2");
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        ImGui::TreePop(); // End of Root Folder
    }
    ImGui::End();
}