#include "MainView.hpp"

#include <ranges>
#include "Instance.hpp"

UntitledGameSystemManager::MainView::MainView()
{

}

void UntitledGameSystemManager::MainView::begin()
{
    beginAutohandle();

}

void UntitledGameSystemManager::MainView::tick(float deltaTime)
{
    tickAutohandle(deltaTime);

    ImGui::Begin("Main", (void*)nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar);
    auto* inst = (Instance*)UImGui::Instance::getGlobal();

    if (inst->selectedContainer != nullptr)
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::MenuItem("+ New Pin"))
                inst->pin.state = UIMGUI_COMPONENT_STATE_RUNNING;
            else if (ImGui::MenuItem("- Delete Pin"))
            {
                auto& pins = inst->selectedContainer->pins;

                for (auto it = pins.begin(); it != pins.end();)
                {
                    if (it->second)
                        it = pins.erase(it);
                    else
                        ++it;
                }

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
                            YAML::Node tmp;
                            for (auto& f : pins)
                                tmp.push_back(f.first);
                            if (tmp.IsNull())
                                tmp.push_back("steam");

                            a["pins"] = tmp;
                            break;
                        }
                    }
                }
                o["containers"] = cont;

                std::ofstream file(inst->configDir + "config/layout.yaml");
                file << o;
            }
            else if (ImGui::MenuItem("* Refresh"))
                inst->loadConfigData();
            else if (ImGui::MenuItem("+ Generate script"))
            {
                auto& pins = inst->selectedContainer->pins;
                for (auto& a : pins)
                    if (a.second)
                        ; // TODO: Generate script for pins here, this includes desktop files, preferably with icons included
            }

            ImGui::EndMenuBar();

            // This is just for keeping track of ids
            size_t i = 0;
            if (ImGui::BeginTable("MainTable", 2))
            {
                ImGui::TableNextColumn();
                for (auto& pin : inst->selectedContainer->pins)
                {
                    ImGui::PushID(static_cast<int>(i));

                    ImGui::Checkbox("##selected?", &pin.second);
                    ImGui::SameLine();
                    ImGui::Text("%s", pin.first.c_str());

                    ImGui::TableNextColumn();

                    if (ImGui::Button("Run##buttonPlay"))
                        LXDExec(inst->selectedContainer->name.data(), ("su{{b}}ubuntu{{b}}-c{{b}}" + pin.first + " & disown").data(), false);

                    ImGui::TableNextColumn();
                    ImGui::PopID();

                    i++;
                }
                ImGui::EndTable();
            }
        }
    }
    ImGui::End();
}

void UntitledGameSystemManager::MainView::end()
{
    endAutohandle();

}

UntitledGameSystemManager::MainView::~MainView()
{

}