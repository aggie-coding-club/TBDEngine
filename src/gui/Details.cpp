// Code for Details window
#include "imgui.h"
#include "gui/Custom_Widgets.h"
#include <glm/vec3.hpp>

void ShowDetails(std::shared_ptr<GameObject> object)
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

    auto objTransform = std::dynamic_pointer_cast<Transform>( object->components.at(0) );

    ImGui::Begin("Details",nullptr,window_flags);

    if (ImGui::TreeNode("Transform")){
        ImGui::Text("Position");
        ImGui::SameLine();
        ImGui::DragFloat3("##Position", &objTransform->position[0], 0.001f, 0.0f, 0.0f, "%.3f");

        ImGui::Text("Rotation");
        ImGui::SameLine();
        ImGui::DragFloat3("##Rotation", &objTransform->rotation[0], 0.001f, 0.0f, 0.0f, "%.3f");

        ImGui::Text("Scale");
        ImGui::SameLine();
        ImGui::DragFloat3("##Scale", &objTransform->scale[0], 0.001f, 0.0f, 0.0f, "%.3f");

        ImGui::TreePop();
    }

    ImGui::End();
}