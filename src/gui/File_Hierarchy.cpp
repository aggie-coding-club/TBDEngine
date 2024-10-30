void ShowFileHierarchy()
{
    ImGui::Begin("Hierarchy");
    if (ImGui::TreeNode("Root Folder"))
    {
        // Folder 1
        if (ImGui::TreeNode("Folder 1"))
        {
            ImGui::Selectable("File 1.1");
            ImGui::Selectable("File 1.2");

            if (ImGui::TreeNode("Subfolder 1.1"))
            {
                ImGui::Selectable("File 1.1.1");
                ImGui::Selectable("File 1.1.2");
                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        // Folder 2
        if (ImGui::TreeNode("Folder 2"))
        {
            ImGui::Selectable("File 2.1");
            ImGui::Selectable("File 2.2");
            ImGui::TreePop();
        }

        // Folder 3
        if (ImGui::TreeNode("Folder 3"))
        {
            if (ImGui::TreeNode("Subfolder 3.1"))
            {
                ImGui::Selectable("File 3.1.1");
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Subfolder 3.2"))
            {
                ImGui::Selectable("File 3.2.1");
                ImGui::Selectable("File 3.2.2");
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        ImGui::TreePop(); // End of Root Folder
    }
    ImGui::End();
}