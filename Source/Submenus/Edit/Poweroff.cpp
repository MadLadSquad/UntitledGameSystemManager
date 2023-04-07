#include "Poweroff.hpp"
#include "Instance.hpp"

UntitledLinuxGameManager::Poweroff::Poweroff()
{
    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledLinuxGameManager::Poweroff::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::Poweroff::tick(float deltaTime)
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
                inst->bWorkerActive = true;
                inst->worker = std::thread([&]() -> void
                {
                    if (LXDStopContainer(inst->selectedContainer->name.data()) != 0)
                    {
                        Logger::log("Failed to power off the following container: ", UVKLog::UVK_LOG_TYPE_ERROR,
                                    inst->selectedContainer->name, " Error: ", LXDGetError());
                        UImGui::Instance::shutdown();
                    }
                    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
                    ((Instance*)UImGui::Instance::getGlobal())->bFinishedExecution = true;
                });
            }
            ImGui::SameLine();
            if (!inst->bWorkerActive && ImGui::Button("Close##info"))
                state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
            ImGui::EndPopup();
        }
    }
}

void UntitledLinuxGameManager::Poweroff::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::Poweroff::~Poweroff()
{

}

