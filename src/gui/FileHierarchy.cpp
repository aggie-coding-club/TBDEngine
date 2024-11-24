// Code for File Hierarchy

#include "gui/FileHierarchy.h"
void FileHierarchy::ShowFileHierarchy(GameEngine *gameEngine, const std::vector<std::shared_ptr<GameObject> > &Objects, bool &showAddObject) {

    // Remove Decorations for the window
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoResize;

    // Set Window width
    ImGui::SetNextWindowSize(size);
    // Set window's position
    ImGui::SetNextWindowPos(pos);
    std::shared_ptr<Scene> scene = gameEngine->GetCurrScene();

    // Create the window
    ImGui::Begin("Hierarchy",nullptr, window_flags);
    if(ImGui::Button("+")) {
        showAddObject = true;
    }
    ImGui::SameLine();
    if (ImGui::TreeNode("Root Folder"))
    {
        // Camera
        if(ImGui::Selectable("Camera")) {
            scene->selectedGameObj = nullptr;
            scene->cameraSelected = true;
        }
        // All other objects in the game
        for (const auto& obj : Objects) {
            if(ImGui::Selectable(obj->name.c_str())) {
#ifndef _USE_SCENE_
                gameEngine->selectedGameObj = obj;
#endif
                scene->selectedGameObj = obj;
                scene->cameraSelected = false;
            }
        }
        ImGui::TreePop(); // End of Root Folder
    }
    ImGui::End();
}