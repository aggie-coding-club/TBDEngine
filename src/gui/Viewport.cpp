#include "gui/Viewport.h"

void Viewport::ShowViewport(unsigned int& textureColorbuffer, unsigned int& rbo){

    ImGui::Begin("Viewport");

    ImVec2 size = ImGui::GetContentRegionAvail();

    ImGui::SetNextWindowPos(ImVec2(0, 29));

    // Display the texture
    ImGui::Image((void*)(intptr_t)textureColorbuffer, size, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}