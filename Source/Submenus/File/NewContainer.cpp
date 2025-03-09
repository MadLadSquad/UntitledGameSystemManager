#include "NewContainer.hpp"
#include "Instance.hpp"

UntitledGameSystemManager::NewContainer::NewContainer() noexcept
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::NewContainer::begin() noexcept
{
    beginAutohandle();

}

void UntitledGameSystemManager::NewContainer::tick(const float deltaTime)
{
    tickAutohandle(deltaTime);
    auto* inst = static_cast<Instance*>(UImGui::Instance::getGlobal());

    if (!ImGui::IsPopupOpen("New container"))
        ImGui::OpenPopup("New container");
    if (ImGui::BeginPopupModal("New container", static_cast<bool*>(nullptr)))
    {
        if (inst->bFinishedExecution)
        {
            bStartExecuting = false;
            inst->bFinishedExecution = false;
        }

        static UImGui::FString name;

        if (bStartExecuting)
        {
            ImGui::TextWrapped("Started creating a new container. Output is written to the terminal.");
            ImGui::TextWrapped("Current step: %s", currentEvent.c_str());
        }
        else
        {
            ImGui::Text("Container name: ");
            ImGui::SameLine();
            ImGui::InputText("##containernameinputbox", &name);
        }

        if (!inst->bWorkerActive && !bStartExecuting && !name.empty())
        {
            if (ImGui::Button("New container##button"))
            {
                inst->bWorkerActive = true;
                inst->worker = std::thread([this, inst]() -> void
                {
                    // We can't just append because multithreading is retarded, instead, we need to copy the data and
                    // then append, otherwise we get a logic error exception thrown.
                    mutex.lock();

                    UImGui::FString dir = inst->configDir + "scripts/ugm-cli-install.sh";
                    UImGui::FString localName = name;

                    const UImGui::FString type = UImGui::Renderer::getGPUName()[0] == 'N' ? "N" : "M";
                    const UImGui::FString version = UImGui::Renderer::getDriverVersion();

                    bStartExecuting = true;
                    currentEvent = "Creating a new container!";

                    mutex.unlock();
                    INCUS_RUN(IncusNewContainer, localName.data(), "create", (char*)"archlinux");

                    LOCK(currentEvent = "Uploading installation script to container!");
                    INCUS_RUN(IncusPushFile, localName.data(), "copy file", (char*)"/root/ugm-cli-install.sh", dir.data());

                    LOCK(currentEvent = "Checking for network connection!");
                    INCUS_RUN(IncusExec, localName.data(), "check for network connection in", (char*)"bash{{b}}-c{{b}}ping -c 5 google.com || ping -c 5 google.com", true);

                    LOCK(currentEvent = "Restarting container!");
                    INCUS_RUN(IncusRestartContainer, localName.data(), "restart");

                    LOCK(currentEvent = "Running installation script, may take more than 20 minutes!");
                    INCUS_RUN(IncusExec, localName.data(), "execute installation script", ("bash{{b}}-c{{b}}/root/ugm-cli-install.sh " + type + " " + version).data(), true);

                    LOCK(currentEvent = "Restarting container, finalising installation!");
                    INCUS_RUN(IncusRestartContainer, localName.data(), "restart");

                    LOCK(
                        YAML::Node out{};
                        out["container"] = name;
                        out["pins"].push_back("steam");
                        out["pins"].push_back("lutris");
                        out["pins"].push_back("firefox");

                        YAML::Node o = inst->loadConfigGeneric();
                        if (o["containers"])
                        {
                            o["containers"].SetStyle(YAML::EmitterStyle::Block);
                            o["containers"].push_back(out);
                        }
                        inst->outputConfig(o);

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

void UntitledGameSystemManager::NewContainer::end() noexcept
{
    endAutohandle();
}