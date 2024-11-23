#include "imgui.h"
#include "gui/AddObjectWindow.h"
#include "core/camera.h"
#include <glm/common.hpp>

void AddObjectWindow::showAddObjectWindow(GameEngine *engine, bool &ShowAddObject){

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

    switch(objectType){
        case 1:
            ImGui::Text("Game object selected");
            ImGui::Text("Location");
            ImGui::SameLine();
            char locationbuffer[128];
            strncpy(locationbuffer, location.c_str(), sizeof(locationbuffer));
            locationbuffer[127] = '\0';
            if(ImGui::InputText("##Location", locationbuffer, sizeof(locationbuffer))) {
                location = locationbuffer;
            }

            ImGui::Text("Name");
            ImGui::SameLine();
            char nameBuffer[128];
            strncpy(nameBuffer, name.c_str(), sizeof(nameBuffer));
            nameBuffer[127] = '\0';
            if(ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer))) {
                name = nameBuffer;
            }
            break;

        case 2:
            ImGui::Text("Light object selected");
            break;
        case 3:
            ImGui::Text("Camera object selected");
            break;
        default:
            ImGui::Text("No object type selected");
    }



    if(ImGui::Button("Add Object")) {
        switch (objectType) {
            case 1:
                if(location != "") {
                    engine->addGameObject(location, name);
                }
                break;
            default:
                ImGui::Text("Hallo");
                break;
        }
        ShowAddObject = false;
        objectType = 0;
    }
    ImGui::End();
};
