#include "SidePanel.hpp"
#include "Instance.hpp"

UntitledLinuxGameManager::SidePanel::SidePanel()
{

}

void UntitledLinuxGameManager::SidePanel::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::SidePanel::tick(float deltaTime)
{
    tickAutohandle(deltaTime);

    ImGui::Begin("Sidebar", (bool*)nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar);

    auto* inst = (Instance*)UImGui::Instance::getGlobal();
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::MenuItem("* Refresh"))
        {
            inst->loadConfigData();
            inst->selectedContainer = nullptr;
        }

        if (ImGui::BeginMenu("* Power"))
        {
            if (ImGui::MenuItem("* Power on") && inst->selectedContainer != nullptr)
                inst->poweron.state = UImGui::UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("* Power off"))
                inst->poweroff.state = UImGui::UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("* Restart"))
                inst->restart.state = UImGui::UIMGUI_COMPONENT_STATE_RUNNING;
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("* More"))
        {
            if (ImGui::MenuItem("+ New"))
                inst->newContainer.state = UImGui::UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("- Delete"))
                ; // TODO: Delete container here
            else if (ImGui::MenuItem("* Update"))
                inst->update.state = UImGui::UIMGUI_COMPONENT_STATE_RUNNING;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    bool bSelected;
    for (auto& a : inst->containers)
    {
        bSelected = false;
        if (inst->selectedContainer == &a)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.87f, 0.64f, 0.03, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f, 0.94f, 0.30f, 1.0f });
            bSelected = true;
        }
        else
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f, });

        if (ImGui::Button(a.name.c_str(), { ImGui::GetContentRegionAvail().x, 0 }))
            inst->selectedContainer = &a;

        if (bSelected)
            ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }
    ImGui::End();
}

void UntitledLinuxGameManager::SidePanel::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::SidePanel::~SidePanel()
{

}

