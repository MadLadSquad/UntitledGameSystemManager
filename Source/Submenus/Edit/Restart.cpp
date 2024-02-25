#include "Restart.hpp"
#include "Instance.hpp"

UntitledGameSystemManager::Restart::Restart()
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::Restart::begin()
{
    beginAutohandle();

}

void UntitledGameSystemManager::Restart::tick(float deltaTime)
{
    tickAutohandle(deltaTime);
    auto* inst = (Instance*)UImGui::Instance::getGlobal();
    if (inst->selectedContainer != nullptr)
    {
        if (!ImGui::IsPopupOpen("Restart"))
            ImGui::OpenPopup("Restart");
        if (ImGui::BeginPopupModal("Restart", (bool*)nullptr))
        {
            ImGui::TextWrapped("This action will force the current container to restart! This CANNOT be reversed!");

            if (!inst->bWorkerActive && ImGui::Button("Restart##button"))
            {
                INCUS_RUN_AND_CLOSE(IncusRestartContainer, "restart");
            }
            ImGui::SameLine();
            if (!inst->bWorkerActive && ImGui::Button("Close##info"))
                state = UIMGUI_COMPONENT_STATE_PAUSED;
            ImGui::EndPopup();
        }
    }
}

void UntitledGameSystemManager::Restart::end()
{
    endAutohandle();

}

UntitledGameSystemManager::Restart::~Restart()
{

}

