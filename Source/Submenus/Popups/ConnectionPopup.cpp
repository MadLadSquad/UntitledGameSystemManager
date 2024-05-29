#include "ConnectionPopup.hpp"

UntitledGameSystemManager::ConnectionPopup::ConnectionPopup()
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::ConnectionPopup::begin()
{
    beginAutohandle();

}

void UntitledGameSystemManager::ConnectionPopup::tick(float deltaTime)
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

void UntitledGameSystemManager::ConnectionPopup::end()
{
    endAutohandle();

}

UntitledGameSystemManager::ConnectionPopup::~ConnectionPopup()
{

}