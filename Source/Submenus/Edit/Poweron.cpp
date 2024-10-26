#include "Poweron.hpp"
#include "Instance.hpp"

UntitledGameSystemManager::Poweron::Poweron() noexcept
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::Poweron::begin() noexcept
{
    beginAutohandle();

}

void UntitledGameSystemManager::Poweron::tick(const float deltaTime) noexcept
{
    tickAutohandle(deltaTime);

    auto* inst = static_cast<Instance*>(UImGui::Instance::getGlobal());
    if (inst->selectedContainer != nullptr)
    {
        if (!ImGui::IsPopupOpen("Power on"))
            ImGui::OpenPopup("Power on");
        if (ImGui::BeginPopupModal("Power on", static_cast<bool*>(nullptr)))
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

void UntitledGameSystemManager::Poweron::end() noexcept
{
    endAutohandle();

}