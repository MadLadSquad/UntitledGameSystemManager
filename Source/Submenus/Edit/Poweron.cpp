#include "Poweron.hpp"
#include "Instance.hpp"

UntitledLinuxGameManager::Poweron::Poweron()
{
    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledLinuxGameManager::Poweron::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::Poweron::tick(float deltaTime)
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
                inst->bWorkerActive = true;
                inst->worker = std::thread([&]() -> void
                {
                    if (LXDStartContainer(inst->selectedContainer->name.data()) != 0)
                    {
                        Logger::log("Failed to power on the following container: ", UVKLog::UVK_LOG_TYPE_ERROR,
                                    inst->selectedContainer->name, " Error: ", LXDGetError());
                        UImGui::Instance::shutdown();
                    }
                    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
                    ((Instance*)UImGui::Instance::getGlobal())->bFinishedExecution = true;
                });
            }

            if (!inst->bWorkerActive && ImGui::Button("Close##info"))
                state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
            ImGui::EndPopup();
        }
    }
}

void UntitledLinuxGameManager::Poweron::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::Poweron::~Poweron()
{

}

