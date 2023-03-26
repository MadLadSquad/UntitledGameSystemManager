#include "MainBar.hpp"
#include "Instance.hpp"

UntitledLinuxGameManager::MainBar::MainBar()
{

}

void UntitledLinuxGameManager::MainBar::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::MainBar::tick(float deltaTime)
{
    tickAutohandle(deltaTime);
    if (ImGui::BeginMainMenuBar())
    {
        // Get the instance from the global
        auto* inst = (Instance*)Instance::getGlobal();
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Container"))
                inst->newContainer.state = UImGui::UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("Exit"))
                inst->exitWidget.state = UImGui::UIMGUI_COMPONENT_STATE_RUNNING;
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Power off"))
                inst->poweroff.state = UImGui::UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("Power on"))
                inst->poweron.state = UImGui::UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("Restart"))
                inst->restart.state = UImGui::UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("New pin"))
                inst->pin.state = UImGui::UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("Update"))
                inst->update.state = UImGui::UIMGUI_COMPONENT_STATE_RUNNING;
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("More"))
        {
            if (ImGui::MenuItem("About us"))
                inst->about.state = UImGui::UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("Help"))
                inst->help.state = UImGui::UIMGUI_COMPONENT_STATE_RUNNING;
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

}

void UntitledLinuxGameManager::MainBar::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::MainBar::~MainBar()
{

}

