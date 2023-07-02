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

        static std::string name;
        static std::string gpupreview;
        static char gpuTypeArgument;

        if (bStartExecuting)
        {
            ImGui::TextWrapped("Started creating a new container");
            ImGui::TextWrapped("Current step: %s", currentEvent.c_str());
        }
        else
        {
            ImGui::Text("Container name: ");
            ImGui::SameLine();
            ImGui::InputText("##containernameinputbox", &name);

            ImGui::Text("GPU Vendor: ");
            ImGui::SameLine();
            if (ImGui::BeginCombo("##selectgpu", gpupreview.c_str()))
            {
                if (ImGui::MenuItem("NVidia"))
                {
                    gpupreview = "NVidia";
                    gpuTypeArgument = 'N';
                }
                else if (ImGui::MenuItem("AMD"))
                {
                    gpupreview = "AMD";
                    gpuTypeArgument = 'M';
                }
                ImGui::EndCombo();
            }
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

                    dir += "scripts/ugm-cli-install.sh";

                    bStartExecuting = true;

                    UImGui::FString version;

                    {
                        const std::lock_guard<std::mutex> lock(mutex);
                        currentEvent = "Creating a new container!";
                        version = UImGui::Renderer::getDriverVersion();
                    }
                    if (LXDNewContainer(name.data(), (char*)"archlinux") != 0)
                    {
                        Logger::log("Failed to create the following container: ", UVKLog::UVK_LOG_TYPE_ERROR,
                                    name, " Error: ", LXDGetError());
                        UImGui::Instance::shutdown();
                    }

                    {
                        const std::lock_guard<std::mutex> lock(mutex);
                        currentEvent = "Uploading installation script to container!";
                    }
                    if (LXDPushFile(name.data(), (char*)"/root/ugm-cli-install.sh", dir.data()) != 0)
                    {
                        Logger::log("Failed to copy file to the following container: ", UVK_LOG_TYPE_ERROR, name,
                                    "Error: ", LXDGetError());
                        UImGui::Instance::shutdown();
                    }

                    {
                        const std::lock_guard<std::mutex> lock(mutex);
                        currentEvent = "Running installation script, may take more than 20 minutes!";
                    }
                    if (LXDExec(name.data(), ("bash{{b}}-c{{b}}/root/ugm-cli-install.sh " + version + " &> /root/out.txt").data(), true) != 0)
                    {
                        Logger::log("Failed to execute installation script of the following container: ", UVK_LOG_TYPE_ERROR, name,
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

                    YAML::Node out;
                    out["container"] = name;
                    out["pins"].push_back("steam");
                    out["pins"].push_back("lutris");
                    out["pins"].push_back("firefox");

                    YAML::Node o;
                    try
                    {
                        o = YAML::LoadFile(conf + "config/layout.yaml");
                    }
                    catch (YAML::BadFile&)
                    {
                        Logger::log("Couldn't open the config file at: ", UVKLog::UVK_LOG_TYPE_ERROR, conf, "config/layout.yaml");
                        std::terminate();
                    }
                    auto cont = o["containers"];
                    if (cont)
                    {
                        cont.push_back(out);
                    }

                    std::ofstream file(conf + "config/layout.yaml");
                    file << o;

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

