#include "Delete.hpp"
#include "Instance.hpp"

UntitledLinuxGameManager::Delete::Delete()
{
    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledLinuxGameManager::Delete::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::Delete::tick(float deltaTime)
{
    tickAutohandle(deltaTime);
    auto* inst = (Instance*)UImGui::Instance::getGlobal();
    if (inst->selectedContainer != nullptr)
    {
        if (!ImGui::IsPopupOpen("Delete"))
            ImGui::OpenPopup("Delete");
        if (ImGui::BeginPopupModal("Delete", (bool*)nullptr))
        {
            ImGui::TextWrapped("This action WILL DELETE the currently selected container! This CANNOT be undone!");

            if (!inst->bWorkerActive && ImGui::Button("Delete##button"))
            {
                inst->bWorkerActive = true;
                inst->worker = std::thread([&]() -> void
                {
                    if (LXDDeleteContainer(inst->selectedContainer->name.data()) != 0)
                    {
                        Logger::log("Failed to delete the following container: ", UVKLog::UVK_LOG_TYPE_ERROR,
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

void UntitledLinuxGameManager::Delete::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::Delete::~Delete()
{

}

