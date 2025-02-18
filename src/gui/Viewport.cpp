#include "gui/Viewport.h"

void Viewport::ShowViewport(unsigned int& textureColorbuffer){

    ImGui::Begin("Viewport");

    ImVec2 DisplaySize = ImGui::GetIO().DisplaySize;
    ImVec2 size = ImVec2(3*DisplaySize.x/4, DisplaySize.y/2);
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(ImVec2(0, 28));

    // Display the texture
    ImGui::Image((void*)(intptr_t)textureColorbuffer, ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}