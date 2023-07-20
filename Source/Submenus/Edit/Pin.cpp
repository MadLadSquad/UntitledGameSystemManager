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
                YAML::Node o;
                try
                {
                    o = YAML::LoadFile(inst->configDir + "config/layout.yaml");
                }
                catch (YAML::BadFile&)
                {
                    Logger::log("Couldn't open the config file at: ", UVKLog::UVK_LOG_TYPE_ERROR, inst->configDir, "config/layout.yaml");
                    std::terminate();
                }
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

                std::ofstream file(inst->configDir + "config/layout.yaml");
                file << o;
                file.close();

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

void UntitledGameSystemManager::Pin::generateScript(const std::string& str, const std::string& command) noexcept
{
    auto* inst = (Instance*)UImGui::Instance::getGlobal();
    std::ofstream out(inst->selectedContainer->name + "-" + str);
    out << "#!/bin/bash" << std::endl << "lxc exec " << inst->selectedContainer->name << " -- bash -c '" << command << "'" << std::endl;

}

