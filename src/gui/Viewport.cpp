#include "imgui.h"
#include <GLFW/glfw3.h>
#include "gui/Custom_Widgets.h"

void ShowViewport(ImVec2 window_Size){

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
    // Create an ImGui window
    ImGui::Begin("Viewport", nullptr, window_flags);

    // Get the position and size of the ImGui window content area
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 size = ImGui::GetContentRegionAvail();

    // End ImGui window but don't render yet
    ImGui::End();

    // Set up OpenGL viewport and scissor area to match the ImGui window
    glViewport((int)pos.x,  window_Size.y - (int)pos.y - size.y, (int)size.x, (int)size.y);
    glEnable(GL_SCISSOR_TEST);
    glScissor((int)pos.x,  window_Size.y - (int)pos.y - size.y, (int)size.x, (int)size.y);
    // Disable scissor test after rendering OpenGL content
    glDisable(GL_SCISSOR_TEST);
}