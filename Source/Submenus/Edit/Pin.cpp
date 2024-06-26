#include "Pin.hpp"
#include "Instance.hpp"

UntitledGameSystemManager::Pin::Pin()
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::Pin::begin()
{
    beginAutohandle();

}

void UntitledGameSystemManager::Pin::tick(float deltaTime)
{
    tickAutohandle(deltaTime);
    auto* inst = (Instance*)UImGui::Instance::getGlobal();
    if (inst->selectedContainer != nullptr)
    {
        if (!ImGui::IsPopupOpen("Pin"))
            ImGui::OpenPopup("Pin");
        if (ImGui::BeginPopupModal("Pin", (bool*)nullptr))
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
                    //cont[inst->selectedContainer->name]["pins"].push_back(cmd);
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

void UntitledGameSystemManager::Pin::end()
{
    endAutohandle();

}

UntitledGameSystemManager::Pin::~Pin()
{

}