#include "Delete.hpp"
#include "Instance.hpp"

UntitledGameSystemManager::Delete::Delete() noexcept
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::Delete::begin() noexcept
{
    beginAutohandle();

}

void UntitledGameSystemManager::Delete::tick(const float deltaTime) noexcept
{
    tickAutohandle(deltaTime);
    auto* inst = static_cast<Instance*>(UImGui::Instance::getGlobal());
    if (inst->selectedContainer != nullptr)
    {
        if (!ImGui::IsPopupOpen("Delete"))
            ImGui::OpenPopup("Delete");
        if (ImGui::BeginPopupModal("Delete", static_cast<bool*>(nullptr)))
        {
            ImGui::TextWrapped("This action WILL DELETE the currently selected container! This CANNOT be undone!");

            if (!inst->bWorkerActive && ImGui::Button("Delete##button"))
            {
                inst->bWorkerActive = true;
                inst->worker = std::thread([inst, this]() -> void
                {
                    mutex.lock();
		            UImGui::FString name = inst->selectedContainer->name;
                    mutex.unlock();
                    INCUS_RUN(IncusDeleteContainer, name.data(), "delete");

                    LOCK(
                        YAML::Node o = inst->loadConfigGeneric();
                        YAML::Node cont = o["containers"];
                        if (cont)
                        {
		                    UImGui::TVector<YAML::Node> containers;
                            for (const YAML::Node& a : cont)
                            {
                                if (a["container"] && a["pins"])
                                {
                                    auto r = a["container"].as<UImGui::FString>();
                                    if (name == r)
                                        continue;
                                }

                                containers.push_back(a);
                            }
                            o["containers"] = containers;
                        }

                        inst->outputConfig(o);

                        state = UIMGUI_COMPONENT_STATE_PAUSED;
                        static_cast<Instance*>(UImGui::Instance::getGlobal())->bFinishedExecution = true;

                        inst->loadConfigData();
                        inst->selectedContainer = nullptr;
                    )
                });
            }
            ImGui::SameLine();
            if (!inst->bWorkerActive && ImGui::Button("Close##info"))
                state = UIMGUI_COMPONENT_STATE_PAUSED;
            ImGui::EndPopup();
        }
    }
}

void UntitledGameSystemManager::Delete::end() noexcept
{
    endAutohandle();

}