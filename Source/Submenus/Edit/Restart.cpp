#include "Restart.hpp"
#include "Instance.hpp"

UntitledGameSystemManager::Restart::Restart() noexcept
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::Restart::begin() noexcept
{
    beginAutohandle();

}

void UntitledGameSystemManager::Restart::tick(const float deltaTime) noexcept
{
    tickAutohandle(deltaTime);
    auto* inst = static_cast<Instance*>(UImGui::Instance::getGlobal());
    if (inst->selectedContainer != nullptr)
    {
        if (!ImGui::IsPopupOpen("Restart"))
            ImGui::OpenPopup("Restart");
        if (ImGui::BeginPopupModal("Restart", static_cast<bool*>(nullptr)))
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

void UntitledGameSystemManager::Restart::end() noexcept
{
    endAutohandle();

}