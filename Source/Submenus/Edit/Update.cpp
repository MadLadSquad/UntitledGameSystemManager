#include "Update.hpp"
#include <Instance.hpp>

UntitledGameSystemManager::Update::Update() noexcept
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::Update::begin() noexcept
{
    beginAutohandle();

}

void UntitledGameSystemManager::Update::tick(const float deltaTime) noexcept
{
    tickAutohandle(deltaTime);
    auto* inst = static_cast<Instance*>(UImGui::Instance::getGlobal());
    if (!ImGui::IsPopupOpen("Update container"))
        ImGui::OpenPopup("Update container");
    if (ImGui::BeginPopupModal("Update container", static_cast<bool*>(nullptr)))
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
                    mutex.lock();

                    UImGui::FString dir = inst->configDir + "scripts/ugm-cli-update.sh";
                    const UImGui::FString type = UImGui::Renderer::getGPUName()[0] == 'N' ? "N" : "M";
                    const UImGui::FString version = UImGui::Renderer::getDriverVersion();
                    UImGui::FString name = container->name;

                    bStartExecuting = true;
                    currentEvent = "Uploading installation script to container!";

                    mutex.unlock();
                    INCUS_RUN(IncusStartContainer, name.data(), "start");

                    LOCK(currentEvent = "Copying update script to container!");
                    INCUS_RUN(IncusPushFile, name.data(), "copy file", (char*)"/root/ugm-cli-update.sh", dir.data());

                    LOCK(currentEvent = "Running the update script(output written to the terminal)!");
                    INCUS_RUN(IncusExec, name.data(), "execute command", ("bash{{b}}-c{{b}}/root/ugm-cli-update.sh " + type + " " + version + "  &> /root/out.txt").data(), true);

                    LOCK(currentEvent = "Restarting container, finalising installation!");
                    INCUS_RUN(IncusRestartContainer, name.data(), "restart")

                    LOCK(
                        state = UIMGUI_COMPONENT_STATE_PAUSED;
                        static_cast<Instance*>(UImGui::Instance::getGlobal())->bFinishedExecution = true;
                    )
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

void UntitledGameSystemManager::Update::end() noexcept
{
    endAutohandle();

}