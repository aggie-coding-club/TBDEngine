#include "imgui.h"
#include "gui/CameraDebugWindow.h"
#include "core/camera.h"
#include <glm/common.hpp>

void CameraDebugWindow::ShowCameraDebugWindow(std::shared_ptr<Camera> camera){
    ImGui::Begin("Camera Debug", nullptr);

    glm::vec3 position = camera->GetPosition();

    ImGui::Text("Position");
    ImGui::SameLine();
    ImGui::DragFloat3("##Position", &position[0], 0.001f,0,0, "%.3f");

    camera->SetPosition(position);

    glm::vec3 rotation = camera->GetRotation();

    ImGui::Text("Rotation");
    ImGui::SameLine();
    ImGui::DragFloat3("##Rotation", &rotation[0], 0.001f, 0, 0, "%.3f");

    camera->SetRotation(rotation);

    ImGui::End();
};