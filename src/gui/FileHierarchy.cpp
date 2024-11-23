// Code for File Hierarchy

#include "gui/FileHierarchy.h"
void FileHierarchy::ShowFileHierarchy(GameEngine *gameEngine, const std::vector<std::shared_ptr<GameObject> > &Objects, bool &showAddObject) {

    // Remove Decorations for the window
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_AlwaysVerticalScrollbar;

    ImVec2 DisplaySize = ImGui::GetIO().DisplaySize;

    float window_width = DisplaySize.x / 4;
    float window_height = DisplaySize.y / 2;
    ImVec2 position(DisplaySize.x - window_width,29);

    // Set Window width
    ImGui::SetNextWindowSize(ImVec2(window_width,window_height));
    // Set window's position
    ImGui::SetNextWindowPos(position);

    // Create the window
    ImGui::Begin("Hierarchy",nullptr, window_flags);
    if(ImGui::Button("+")) {
        showAddObject = true;
    }
    ImGui::SameLine();
    if (ImGui::TreeNode("Root Folder"))
    {
        for (const auto& obj : Objects) {
            if(ImGui::Selectable(obj->name.c_str())) {
#ifndef _USE_SCENE_
                gameEngine->selectedGameObj = obj;
#endif
                gameEngine->GetCurrScene()->selectedGameObj = obj;
            }
        }

        ImGui::TreePop(); // End of Root Folder
    }
    ImGui::End();
}