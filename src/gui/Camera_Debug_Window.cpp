#include "imgui.h"
#include "gui/Custom_Widgets.h"
#include "core/camera.h"
#include <glm/common.hpp>

void ShowCameraDebugWindow(Camera *camera){
    ImGui::Begin("Camera Debug", nullptr);

    glm::vec3 camEye = camera->GetEye();

    ImGui::Text("Camera Eye");
    ImGui::SameLine();
    ImGui::DragFloat3("##CamEye", &camEye[0], 0.1f, -100.0f, 100.0f, "%.3f");

    camera->SetEye(camEye);

    glm::vec3 camCenter = camera->GetCenter();

    ImGui::Text("Camera Center");
    ImGui::SameLine();
    ImGui::DragFloat3("##CamCenter", &camCenter[0], 0.1f, -100.0f, 100.0f, "%.3f");

    camera->SetCenter(camCenter);

    glm::vec3 camUp = camera->GetUpVec();

    ImGui::Text("Camera Up Vector");
    ImGui::SameLine();
    ImGui::DragFloat3("##CamUpVect", &camUp[0], 0.001f, -100.0f, 100.0f, "%.3f");

    camUp = glm::normalize(camUp);

    camera->SetUpVec(camUp);

    ImGui::End();
};