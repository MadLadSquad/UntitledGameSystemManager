#include "NewContainer.hpp"

UntitledLinuxGameManager::NewContainer::NewContainer()
{
    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledLinuxGameManager::NewContainer::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::NewContainer::tick(float deltaTime)
{
    tickAutohandle(deltaTime);
    if (!ImGui::IsPopupOpen("New Warning"))
        ImGui::OpenPopup("New Warning");
    if (ImGui::BeginPopupModal("New Warning", &state))
    {
        ImGui::TextWrapped("This will launch the UntitledLinuxGameManager GUI installer!");

        if (ImGui::Button("Cancel##nnew"))
            state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;

        ImGui::SameLine();

        if (ImGui::Button("Start##new"))
        {
            state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
            // TODO: Create a new container here
        }
        ImGui::EndPopup();
    }
}

void UntitledLinuxGameManager::NewContainer::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::NewContainer::~NewContainer()
{

}

