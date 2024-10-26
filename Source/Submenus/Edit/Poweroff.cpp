#include "Poweroff.hpp"
#include "Instance.hpp"

UntitledGameSystemManager::Poweroff::Poweroff() noexcept
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::Poweroff::begin() noexcept
{
    beginAutohandle();

}

void UntitledGameSystemManager::Poweroff::tick(const float deltaTime) noexcept
{
    tickAutohandle(deltaTime);

    auto* inst = static_cast<Instance*>(UImGui::Instance::getGlobal());
    if (inst->selectedContainer != nullptr)
    {
        if (!ImGui::IsPopupOpen("Power off"))
            ImGui::OpenPopup("Power off");
        if (ImGui::BeginPopupModal("Power off", static_cast<bool*>(nullptr)))
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

void UntitledGameSystemManager::Poweroff::end() noexcept
{
    endAutohandle();

}