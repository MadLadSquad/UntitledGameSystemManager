#include "Exit.hpp"

UntitledLinuxGameManager::Exit::Exit()
{
    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledLinuxGameManager::Exit::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::Exit::tick(float deltaTime)
{
    tickAutohandle(deltaTime);
    if (!ImGui::IsPopupOpen("Exit"))
        ImGui::OpenPopup("Exit");
    if (ImGui::BeginPopupModal("Exit", &state))
    {
        ImGui::TextWrapped("Are you sure you want to exit the UntitledLinuxGameManager? All running containers will not be powered off!");

        if (ImGui::Button("Exit##exit"))
            UImGui::Instance::shutdown();

        ImGui::SameLine();
        if (ImGui::Button("Cancel##exit"))
            state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;

        ImGui::EndPopup();
    }
}

void UntitledLinuxGameManager::Exit::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::Exit::~Exit()
{

}

