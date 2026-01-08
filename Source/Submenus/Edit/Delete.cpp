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
                        ryml::Tree tree{};
                        auto o = inst->loadConfigGeneric(tree);

                        auto cont = o["containers"];
                        if (keyValid(cont) && cont.is_seq())
                        {
                            cont.clear_style();

                            for (auto a : cont.children())
                            {
                                auto c = a["container"];
                                auto pins = a["pins"];

                                if (keyValid(c) && keyValid(pins) && pins.is_seq())
                                {
                                    UImGui::FString r{};
                                    c >> r;
                                    if (r == name)
                                    {
                                        cont.remove_child(a);
                                        break;
                                    }
                                }
                            }
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