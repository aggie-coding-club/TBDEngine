// Code for Details window
#include "imgui.h"
#include "gui/Custom_Widgets.h"
#include <glm/vec3.hpp>

void ShowDetails()
{
    // Remove Decorations for the window
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_AlwaysVerticalScrollbar;

    ImVec2 DisplaySize = ImGui::GetIO().DisplaySize;

    ImVec2 size(DisplaySize.x / 4, DisplaySize.y / 2.5);
    ImVec2 pos(DisplaySize.x - size.x,29 + DisplaySize.y / 2.5);

    // Set Window width
    ImGui::SetNextWindowSize(size);
    // Set window's position
    ImGui::SetNextWindowPos(pos);
    glm::vec3 Position(0);
    glm::vec3 Rotation(0);
    glm::vec3 Scale(0);
    ImGui::Begin("Details",nullptr,window_flags);
    
    ImGui::InputFloat3("Position", &Position[0], "%.3f");

    ImGui::InputFloat3("Rotation", &Rotation[0], "%.3f");

    ImGui::InputFloat3("Scale", &Scale[0], "%.3f");

    ImGui::End();
}