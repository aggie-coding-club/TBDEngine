#include "imgui.h"
#include "gui/Custom_Widgets.h"
#include "core/camera.h"
#include <glm/common.hpp>

void ShowAddObjectWindow(GameEngine *engine, bool &ShowAddObject, int &objectType){

    // Set add object;
    ImGui::Begin("Add Object Window", NULL);

    if (ImGui::Button("Game Object")) {
        objectType = 1;
    }
    ImGui::SameLine();
    if (ImGui::Button("Light")) {
        objectType = 2;
    }
    ImGui::SameLine();
    if (ImGui::Button("Camera")) {
        objectType = 3;
    }

    std::string objectTypeSelected;
    switch(objectType){
        case 1:
            objectTypeSelected = "Game object selected";
            ImGui::Text("Location");
            ImGui::SameLine();
            ImGui::InputText("##Location", "A", sizeof("A"));
            ImGui::Text("Name");
            ImGui::SameLine();
            ImGui::InputText("##Name", "B", sizeof("B"));

            break;
        case 2:
            objectTypeSelected = "Light object selected";
            break;
        case 3:
            objectTypeSelected = "Camera object selected";
            break;
        default:
            objectTypeSelected = "No object type selected";
    }
    ImGui::Text(objectTypeSelected.data());

    if(ImGui::Button("Add Object")) {
        switch (objectType) {
            case 1:
                engine->addGameObject();
        }
        ShowAddObject = false;
        objectType = 0;
    }
    ImGui::End();
};

void AddingGameObject(GameEngine *engine) {


}