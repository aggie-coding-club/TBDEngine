// Code for Details window

#include "gui/Details.h"

void ShowTransform(std::shared_ptr<Transform> &object_transform){
    if (ImGui::TreeNode("Transform")){
        ImGui::Text("Position");
        ImGui::SameLine();
        ImGui::DragFloat3("##Position", &object_transform->position[0], 0.001f, 0.0f, 0.0f, "%.3f");

        ImGui::Text("Rotation");
        ImGui::SameLine();
        ImGui::DragFloat3("##Rotation", &object_transform->rotation[0], 0.001f * 5.f, 0.0f, 0.0f, "%.3f");

        ImGui::Text("Scale");
        ImGui::SameLine();
        ImGui::DragFloat3("##Scale", &object_transform->scale[0], 0.001f, 0.0f, 0.0f, "%.3f");

        ImGui::TreePop();
    }
}

void ShowMaterial(std::shared_ptr<Material> &object_material) {
    if (ImGui::TreeNode("Material")){
        ImGui::Text("Ambient");
        ImGui::SameLine();
        ImGui::ColorEdit3("##Ambient", &object_material->ambient[0]);

        ImGui::Text("Diffuse");
        ImGui::SameLine();
        ImGui::ColorEdit3("##Diffuse", &object_material->diffuse[0]);

        ImGui::Text("Specular");
        ImGui::SameLine();
        ImGui::ColorEdit3("##Specular", &object_material->specular[0]);

        ImGui::Text("Shininess");
        ImGui::SameLine();
        ImGui::DragFloat("##Shininess", &object_material->shininess, 0.1f, 0.0f, 0.0f, "%.1f", ImGuiSliderFlags_ClampOnInput);
        object_material->shininess = std::max(0.f, object_material->shininess);

        ImGui::TreePop();
    }
}

void Details::ShowDetails(std::shared_ptr<Scene> scene, Camera *camera)
{
    // Remove Decorations for the window
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoResize;

    // Set Window width
    ImGui::SetNextWindowSize(size);
    // Set window's position
    ImGui::SetNextWindowPos(pos);

    auto object = scene->selectedGameObj;

    ImGui::Begin("Details",nullptr,window_flags);
    if (scene->cameraSelected) {

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

    }
    else if(object)
    {
        auto objectName = &object->name;
        char nameBuffer[128];
        strncpy(nameBuffer, objectName->c_str(), sizeof(nameBuffer));
        nameBuffer[127] = '\0';
        ImGui::Text("Name");
        ImGui::SameLine();
        if(ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer))) {
            object->name = nameBuffer;
        }

        for(const auto& objComponent : object->components) {

            // Try to cast component to a transform
            if(auto objTransform = std::dynamic_pointer_cast<Transform>( objComponent ))
            {
                ShowTransform(objTransform);
            }
            // Try to cast component to a Material
            if(auto objMaterial = std::dynamic_pointer_cast<Material>( objComponent ))
            {
                ShowMaterial(objMaterial);
            }

        }
    }
    else {
        ImGui::Text("No Object Selected");
    }

    ImGui::End();
}