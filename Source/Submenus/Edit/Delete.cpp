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
                    YAML::Node o = inst->loadConfigGeneric();
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

                    inst->outputConfig(o);

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

