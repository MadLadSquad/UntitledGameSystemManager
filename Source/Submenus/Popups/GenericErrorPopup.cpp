#include "GenericErrorPopup.hpp"

UntitledGameSystemManager::GenericErrorPopup::GenericErrorPopup()
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::GenericErrorPopup::begin()
{
    beginAutohandle();

}

void UntitledGameSystemManager::GenericErrorPopup::tick(float deltaTime)
{
    tickAutohandle(deltaTime);
    if (!ImGui::IsPopupOpen(popupName.c_str()))
        ImGui::OpenPopup(popupName.c_str());
    if (ImGui::BeginPopupModal(popupName.c_str(), &state))
    {
        ImGui::TextWrapped("%s", popupString.c_str());

        if (ImGui::Button("Close##Aboutus"))
            state = UIMGUI_COMPONENT_STATE_PAUSED;
        ImGui::EndPopup();
    }
}

void UntitledGameSystemManager::GenericErrorPopup::end()
{
    endAutohandle();

}

UntitledGameSystemManager::GenericErrorPopup::~GenericErrorPopup()
{

}