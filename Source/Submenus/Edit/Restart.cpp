#include "Restart.hpp"
#include "Instance.hpp"

UntitledLinuxGameManager::Restart::Restart()
{
    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledLinuxGameManager::Restart::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::Restart::tick(float deltaTime)
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
                inst->bWorkerActive = true;
                inst->worker = std::thread([&]() -> void
                {
                    if (LXDRestartContainer(inst->selectedContainer->name.data()) != 0)
                    {
                        Logger::log("Failed to restart the following container: ", UVKLog::UVK_LOG_TYPE_ERROR,
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

void UntitledLinuxGameManager::Restart::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::Restart::~Restart()
{

}

