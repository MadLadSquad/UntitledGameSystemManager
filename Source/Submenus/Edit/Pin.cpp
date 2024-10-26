#include "Pin.hpp"
#include "Instance.hpp"

UntitledGameSystemManager::Pin::Pin() noexcept
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::Pin::begin() noexcept
{
    beginAutohandle();

}

void UntitledGameSystemManager::Pin::tick(const float deltaTime) noexcept
{
    tickAutohandle(deltaTime);
    auto* inst = static_cast<Instance*>(UImGui::Instance::getGlobal());
    if (inst->selectedContainer != nullptr)
    {
        if (!ImGui::IsPopupOpen("Pin"))
            ImGui::OpenPopup("Pin");
        if (ImGui::BeginPopupModal("Pin", static_cast<bool*>(nullptr)))
        {
            static UImGui::FString cmd;

            ImGui::TextWrapped("This menu creates a new application pin that will be displayed in the game manager!");

            ImGui::Text("Command");
            ImGui::SameLine();
            ImGui::InputText("##command-in", &cmd);

            if (ImGui::Button("Create"))
            {
                YAML::Node o = inst->loadConfigGeneric();
                auto cont = o["containers"];
                if (cont)
                {
                    for (YAML::Node a : cont)
                    {
                        if (a["container"] && a["pins"] && a["container"].as<UImGui::FString>() == inst->selectedContainer->name)
                        {
                            a["pins"].push_back(cmd);
                            break;
                        }
                    }
                }
                o["containers"] = cont;

                inst->outputConfig(o);

                inst->loadConfigData();
                cmd.clear();
                state = UIMGUI_COMPONENT_STATE_PAUSED;
            }
            ImGui::SameLine();
            if (ImGui::Button("Close##info"))
                state = UIMGUI_COMPONENT_STATE_PAUSED;
            ImGui::EndPopup();
        }
    }
}

void UntitledGameSystemManager::Pin::end() noexcept
{
    endAutohandle();

}