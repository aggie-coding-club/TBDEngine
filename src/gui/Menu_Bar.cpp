// Code for menu bar

#include "imgui.h"
#include "gui/Custom_Widgets.h"

void ShowMenuBar(){
    if (ImGui::BeginMainMenuBar())
    {
        // File menu
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) {
                // Action for New
            }
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                // Action for Open
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                // Action for Save
            }
            if (ImGui::MenuItem("Exit", "Alt+F4")) {
                // Action for Exit
            }
            ImGui::EndMenu();
        }

        // Edit menu
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
                // Action for Undo
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
                // Action for Redo
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "Ctrl+X")) {
                // Action for Cut
            }
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {
                // Action for Copy
            }
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {
                // Action for Paste
            }
            ImGui::EndMenu();
        }

        // View menu
        if (ImGui::BeginMenu("View"))
        {
            static bool showToolbar = true;
            ImGui::MenuItem("Show Toolbar", nullptr, &showToolbar);

            static bool showStatusBar = true;
            ImGui::MenuItem("Show Status Bar", nullptr, &showStatusBar);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window")) {
            static bool showWindowbar = true;
            ImGui::MenuItem("Temp", nullptr);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            static bool showHelp = true;
            ImGui::MenuItem("About", nullptr);
            ImGui::MenuItem("FAQ", nullptr);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}