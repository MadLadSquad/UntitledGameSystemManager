#include "Delete.hpp"
#include "Instance.hpp"

UntitledGameSystemManager::Delete::Delete()
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::Delete::begin()
{
    beginAutohandle();

}

void UntitledGameSystemManager::Delete::tick(float deltaTime)
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
                inst->worker = std::thread([inst, this]() -> void
                {
                    std::string name;
                    std::string configDir;
                    {
                        const std::lock_guard<std::mutex> lock(mutex);
                        name = inst->selectedContainer->name;
                        configDir = inst->configDir;
                    }

                    INCUS_RUN(IncusDeleteContainer, name.data(), "delete");

                    const std::lock_guard<std::mutex> lock(mutex);
                    YAML::Node o;
                    try
                    {
                        o = YAML::LoadFile(configDir + "config/layout.yaml");
                    }
                    catch (YAML::BadFile&)
                    {
                        Logger::log("Couldn't open the config file at: ", UVKLog::UVK_LOG_TYPE_ERROR, configDir, "config/layout.yaml");
                        std::terminate();
                    }
                    YAML::Node cont = o["containers"];
                    if (cont)
                    {
                        std::vector<YAML::Node> containers;
                        for (const YAML::Node& a : cont)
                        {
                            if (a["container"] && a["pins"])
                            {
                                auto r = a["container"].as<std::string>();
                                if (name == r)
                                    continue;
                            }

                            containers.push_back(a);
                        }
                        o["containers"] = containers;
                    }

                    std::ofstream file(configDir + "config/layout.yaml");
                    file << o;
                    file.close();

                    state = UIMGUI_COMPONENT_STATE_PAUSED;
                    ((Instance*)UImGui::Instance::getGlobal())->bFinishedExecution = true;

                    inst->loadConfigData();
                    inst->selectedContainer = nullptr;
                });
            }
            ImGui::SameLine();
            if (!inst->bWorkerActive && ImGui::Button("Close##info"))
                state = UIMGUI_COMPONENT_STATE_PAUSED;
            ImGui::EndPopup();
        }
    }
}

void UntitledGameSystemManager::Delete::end()
{
    endAutohandle();

}

UntitledGameSystemManager::Delete::~Delete()
{

}

