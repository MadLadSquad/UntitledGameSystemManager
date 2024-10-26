#include "ConnectionPopup.hpp"

UntitledGameSystemManager::ConnectionPopup::ConnectionPopup() noexcept
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::ConnectionPopup::begin() noexcept
{
    beginAutohandle();

}

void UntitledGameSystemManager::ConnectionPopup::tick(const float deltaTime) noexcept
{
    tickAutohandle(deltaTime);
    if (!ImGui::IsPopupOpen("Couldn't connect to Incus"))
        ImGui::OpenPopup("Couldn't connect to Incus");
    if (ImGui::BeginPopupModal("Couldn't connect to Incus", &state))
    {
        ImGui::TextWrapped("Couldn't connect to Incus. Error: %s", error.c_str());

        if (ImGui::Button("Close##Aboutus"))
            UImGui::Instance::shutdown();
        ImGui::EndPopup();
    }
}

void UntitledGameSystemManager::ConnectionPopup::end() noexcept
{
    endAutohandle();

}