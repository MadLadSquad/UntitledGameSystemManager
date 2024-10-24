#include "NewContainer.hpp"
#include "Instance.hpp"

UntitledGameSystemManager::NewContainer::NewContainer()
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::NewContainer::begin()
{
    beginAutohandle();

}

void UntitledGameSystemManager::NewContainer::tick(float deltaTime)
{
    tickAutohandle(deltaTime);
    auto* inst = (Instance*)UImGui::Instance::getGlobal();

    if (!ImGui::IsPopupOpen("New container"))
        ImGui::OpenPopup("New container");
    if (ImGui::BeginPopupModal("New container", (bool*)nullptr))
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
                inst->worker = std::thread([&]() -> void
                {
                    // We can't just append because multithreading is retarded, instead, we need to copy the data and
                    // then append, otherwise we get a logic error exception thrown.
                    UImGui::FString dir = inst->configDir;
                    UImGui::FString conf = dir;
                    UImGui::FString type;

                    dir += "scripts/ugm-cli-install.sh";
                    bStartExecuting = true;
                    UImGui::FString version;

                    {
                        const std::lock_guard<std::mutex> lock(mutex);
                        currentEvent = "Creating a new container!";
                        version = UImGui::Renderer::getDriverVersion();
                        type = UImGui::Renderer::getGPUName()[0] == 'N' ? "N" : "M";
                    }
                    INCUS_RUN(IncusNewContainer, name.data(), "create", (char*)"archlinux");

                    {
                        const std::lock_guard<std::mutex> lock(mutex);
                        currentEvent = "Uploading installation script to container!";
                    }
                    INCUS_RUN(IncusPushFile, name.data(), "copy file", (char*)"/root/ugm-cli-install.sh", dir.data());
                    IncusExec(name.data(), (char*)"bash{{b}}-c{{b}}ping -c 5 google.com || ping -c 5 google.com", true);
                    IncusRestartContainer(name.data());

                    {
                        const std::lock_guard<std::mutex> lock(mutex);
                        currentEvent = "Running installation script, may take more than 20 minutes!";
                    }
                    INCUS_RUN(IncusExec, name.data(), "execute installation script", ("bash{{b}}-c{{b}}/root/ugm-cli-install.sh " + type + " " + version).data(), true);

                    {
                        const std::lock_guard<std::mutex> lock(mutex);
                        currentEvent = "Restarting container, finalising installation!";
                    }
                    INCUS_RUN(IncusRestartContainer, name.data(), "restart");

                    YAML::Node out;
                    out["container"] = name;
                    out["pins"].push_back("steam");
                    out["pins"].push_back("lutris");
                    out["pins"].push_back("firefox");

                    YAML::Node o = inst->loadConfigGeneric();
                    auto cont = o["containers"];
                    if (cont)
                    {
                        cont.push_back(out);
                    }
                    inst->outputConfig(o);

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

void UntitledGameSystemManager::NewContainer::end()
{
    endAutohandle();
}

UntitledGameSystemManager::NewContainer::~NewContainer()
{

}

