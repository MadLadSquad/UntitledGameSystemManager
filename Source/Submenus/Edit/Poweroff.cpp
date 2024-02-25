#include "Poweroff.hpp"
#include "Instance.hpp"

UntitledGameSystemManager::Poweroff::Poweroff()
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::Poweroff::begin()
{
    beginAutohandle();

}

void UntitledGameSystemManager::Poweroff::tick(float deltaTime)
{
    tickAutohandle(deltaTime);

    auto* inst = (Instance*)UImGui::Instance::getGlobal();
    if (inst->selectedContainer != nullptr)
    {
        if (!ImGui::IsPopupOpen("Power off"))
            ImGui::OpenPopup("Power off");
        if (ImGui::BeginPopupModal("Power off", (bool*)nullptr))
        {
            ImGui::TextWrapped("This action will force the current container to power off! This CANNOT be reversed!");

            if (!inst->bWorkerActive && ImGui::Button("Power off##button"))
            {
                INCUS_RUN_AND_CLOSE(IncusStopContainer, "power off");
            }
            ImGui::SameLine();
            if (!inst->bWorkerActive && ImGui::Button("Close##info"))
                state = UIMGUI_COMPONENT_STATE_PAUSED;
            ImGui::EndPopup();
        }
    }
}

void UntitledGameSystemManager::Poweroff::end()
{
    endAutohandle();

}

UntitledGameSystemManager::Poweroff::~Poweroff()
{

}

