#include "imgui.h"
#include <GLFW/glfw3.h>
#include "gui/Custom_Widgets.h"

void ShowViewport(ImVec2 window_Size){

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

    ImVec2 DisplaySize = ImGui::GetIO().DisplaySize;

    ImVec2 size(DisplaySize.x*3.f/4.f,DisplaySize.y/1.5);
    ImGui::SetNextWindowSize(size);

    ImGui::SetNextWindowPos(ImVec2(0, 29));

    // Create an ImGui window
    ImGui::Begin("Viewport", nullptr, window_flags);

    // Get the position and size of the ImGui window content area
    ImVec2 currPos = ImGui::GetCursorScreenPos();
    ImVec2 currSize = ImGui::GetContentRegionAvail();

    // End ImGui window but don't render yet
    ImGui::End();

    // Set up OpenGL viewport and scissor area to match the ImGui window
    glViewport((int)currPos.x,  window_Size.y - (int)currPos.y - currSize.y, (int)currSize.x, (int)currSize.y);
    glEnable(GL_SCISSOR_TEST);
    glScissor((int)currPos.x,  window_Size.y - (int)currPos.y - currSize.y, (int)currSize.x, (int)currSize.y);
    // Disable scissor test after rendering OpenGL content
    glDisable(GL_SCISSOR_TEST);
}