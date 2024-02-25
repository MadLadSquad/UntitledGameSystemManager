#include "Poweron.hpp"
#include "Instance.hpp"

UntitledGameSystemManager::Poweron::Poweron()
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::Poweron::begin()
{
    beginAutohandle();

}

void UntitledGameSystemManager::Poweron::tick(float deltaTime)
{
    tickAutohandle(deltaTime);

    auto* inst = (Instance*)UImGui::Instance::getGlobal();
    if (inst->selectedContainer != nullptr)
    {
        if (!ImGui::IsPopupOpen("Power on"))
            ImGui::OpenPopup("Power on");
        if (ImGui::BeginPopupModal("Power on", (bool*)nullptr))
        {
            ImGui::TextWrapped("This action will force the current container to power on! This CANNOT be reversed!");

            if (!inst->bWorkerActive && ImGui::Button("Power on##button"))
            {
                INCUS_RUN_AND_CLOSE(IncusStartContainer, "power on");
            }
            ImGui::SameLine();
            if (!inst->bWorkerActive && ImGui::Button("Close##info"))
                state = UIMGUI_COMPONENT_STATE_PAUSED;
            ImGui::EndPopup();
        }
    }
}

void UntitledGameSystemManager::Poweron::end()
{
    endAutohandle();

}

UntitledGameSystemManager::Poweron::~Poweron()
{

}

