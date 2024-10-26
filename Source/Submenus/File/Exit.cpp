#include "Exit.hpp"

UntitledGameSystemManager::Exit::Exit() noexcept
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::Exit::begin() noexcept
{
    beginAutohandle();

}

void UntitledGameSystemManager::Exit::tick(const float deltaTime) noexcept
{
    tickAutohandle(deltaTime);
    if (!ImGui::IsPopupOpen("Exit"))
        ImGui::OpenPopup("Exit");
    if (ImGui::BeginPopupModal("Exit", &state))
    {
        ImGui::TextWrapped("Are you sure you want to exit the UntitledGameSystemManager? All running containers will not be powered off!");

        if (ImGui::Button("Exit##exit"))
            UImGui::Instance::shutdown();

        ImGui::SameLine();
        if (ImGui::Button("Cancel##exit"))
            state = UIMGUI_COMPONENT_STATE_PAUSED;

        ImGui::EndPopup();
    }
}

void UntitledGameSystemManager::Exit::end() noexcept
{
    endAutohandle();

}