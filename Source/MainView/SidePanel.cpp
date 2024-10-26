#include "SidePanel.hpp"
#include "Instance.hpp"

void UntitledGameSystemManager::SidePanel::begin() noexcept
{
    beginAutohandle();

}

void UntitledGameSystemManager::SidePanel::tick(const float deltaTime) noexcept
{
    tickAutohandle(deltaTime);

    ImGui::Begin("Sidebar", static_cast<bool*>(nullptr), ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar);

    auto* inst = static_cast<Instance*>(UImGui::Instance::getGlobal());
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
                inst->poweron.state = UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("* Power off"))
                inst->poweroff.state = UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("* Restart"))
                inst->restart.state = UIMGUI_COMPONENT_STATE_RUNNING;
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("* More"))
        {
            if (ImGui::MenuItem("+ New"))
                inst->newContainer.state = UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("- Delete"))
                inst->del.state = UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("* Update"))
                inst->update.state = UIMGUI_COMPONENT_STATE_RUNNING;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    size_t i = 0; // Generates unique IDs
    for (auto& a : inst->containers)
    {
        bool bSelected = false;
        if (inst->selectedContainer == &a)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.87f, 0.64f, 0.03, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f, 0.94f, 0.30f, 1.0f });
            bSelected = true;
        }
        else
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f, });

        UImGui::FString status = IncusGetState(a.name.data()) == 1 ? " - ON" : " - OFF";

        // Name - ON/OFF(hidden ID)
        if (ImGui::Button((a.name + status + "##" + std::to_string(i).c_str()).c_str(), { ImGui::GetContentRegionAvail().x, 0 }))
            inst->selectedContainer = &a;

        if (bSelected)
            ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ++i;
    }
    ImGui::End();
}

void UntitledGameSystemManager::SidePanel::end() noexcept
{
    endAutohandle();

}