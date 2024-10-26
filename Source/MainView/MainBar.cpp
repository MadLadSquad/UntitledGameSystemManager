#include "MainBar.hpp"
#include "Instance.hpp"

void UntitledGameSystemManager::MainBar::begin() noexcept
{
    beginAutohandle();

}

void UntitledGameSystemManager::MainBar::tick(const float deltaTime) noexcept
{
    tickAutohandle(deltaTime);
    if (ImGui::BeginMainMenuBar())
    {
        // Get the instance from the global
        auto* inst = static_cast<Instance*>(Instance::getGlobal());
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Container"))
                inst->newContainer.state = UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("Exit"))
                inst->exitWidget.state = UIMGUI_COMPONENT_STATE_RUNNING;
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Power off"))
                inst->poweroff.state = UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("Power on"))
                inst->poweron.state = UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("Restart"))
                inst->restart.state = UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("New pin"))
                inst->pin.state = UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("Update"))
                inst->update.state = UIMGUI_COMPONENT_STATE_RUNNING;
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("More"))
        {
            if (ImGui::MenuItem("About us"))
                inst->about.state = UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("Help"))
                inst->help.state = UIMGUI_COMPONENT_STATE_RUNNING;
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

}

void UntitledGameSystemManager::MainBar::end() noexcept
{
    endAutohandle();

}