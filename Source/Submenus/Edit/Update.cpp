#include "Update.hpp"
#include <Instance.hpp>

UntitledGameSystemManager::Update::Update()
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::Update::begin()
{
    beginAutohandle();

}

void UntitledGameSystemManager::Update::tick(float deltaTime)
{
    tickAutohandle(deltaTime);
    auto* inst = (Instance*)UImGui::Instance::getGlobal();
    if (!ImGui::IsPopupOpen("Update container"))
        ImGui::OpenPopup("Update container");
    if (ImGui::BeginPopupModal("Update container", (bool*)nullptr))
    {
        if (inst->bFinishedExecution)
        {
            bStartExecuting = false;
            inst->bFinishedExecution = false;
        }

        if (bStartExecuting)
        {
            ImGui::TextWrapped("Started updating a new container!");
            ImGui::TextWrapped("Current step: %s", currentEvent.c_str());
        }
        else
            ImGui::Text("This will run the update script on your container. After being started, the window will close automatically once the update finishes.");

        auto* container = inst->selectedContainer;

        if (!inst->bWorkerActive && !bStartExecuting && container != nullptr && !container->name.empty())
        {
            if (ImGui::Button("Update container##button"))
            {
                inst->bWorkerActive = true;
                inst->worker = std::thread([container, inst, this]() -> void
                {
                    UImGui::FString dir;
                    UImGui::FString type;

                    bStartExecuting = true;
                    UImGui::FString version;

                    UImGui::FString name;

                    {
                        const std::lock_guard<std::mutex> lock(mutex);
                        currentEvent = "Uploading installation script to container!";

                        dir = inst->configDir;
                        dir += "scripts/ugm-cli-update.sh";

                        version = UImGui::Renderer::getDriverVersion();
                        type = UImGui::Renderer::getGPUName()[0] == 'N' ? "N" : "M";

                        name = container->name;
                    }
                    if (LXDStartContainer(name.data()) != 0)
                    {
                        Logger::log("Failed to start the following container: ", UVK_LOG_TYPE_ERROR, name,
                                    "Error: ", LXDGetError());
                        UImGui::Instance::shutdown();
                    }

                    {
                        const std::lock_guard<std::mutex> lock(mutex);
                        currentEvent = "Copying update script to container!";
                    }
                    if (LXDPushFile(name.data(), (char*)"/root/ugm-cli-update.sh", dir.data()) != 0)
                    {
                        Logger::log("Failed to copy file to the following container: ", UVK_LOG_TYPE_ERROR, name,
                                    "Error: ", LXDGetError());
                        UImGui::Instance::shutdown();
                    }

                    {
                        const std::lock_guard<std::mutex> lock(mutex);
                        currentEvent = "Copying update script to container!";
                    }
                    if (LXDExec(name.data(), ("bash{{b}}-c{{b}}/root/ugm-cli-update.sh " + type + " " + version + "  &> /root/out.txt").data(), true) != 0)
                    {Logger::log("Failed to copy file to the following container: ", UVK_LOG_TYPE_ERROR, name,
                                    "Error: ", LXDGetError());
                        UImGui::Instance::shutdown();
                    }

                    {
                        const std::lock_guard<std::mutex> lock(mutex);
                        currentEvent = "Restarting container, finalising installation!";
                    }
                    if (LXDRestartContainer(name.data()) != 0)
                    {
                        Logger::log("Failed to restart the following container: ", UVK_LOG_TYPE_ERROR, name,
                                    "Error: ", LXDGetError());
                        UImGui::Instance::shutdown();
                    }

                    state = UIMGUI_COMPONENT_STATE_PAUSED;
                    ((Instance*)UImGui::Instance::getGlobal())->bFinishedExecution = true;
                });
            }
            else
                ImGui::SameLine();
        }
        if (!inst->bWorkerActive && !bStartExecuting && ImGui::Button("Close##info"))
            state = UIMGUI_COMPONENT_STATE_PAUSED;
        ImGui::EndPopup();
    }
}

void UntitledGameSystemManager::Update::end()
{
    endAutohandle();

}

UntitledGameSystemManager::Update::~Update()
{

}

