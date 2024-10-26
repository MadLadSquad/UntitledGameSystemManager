#include "MainView.hpp"

#include <ranges>
#include "Instance.hpp"

void UntitledGameSystemManager::MainView::begin() noexcept
{
    beginAutohandle();

}

void UntitledGameSystemManager::MainView::tick(const float deltaTime) noexcept
{
    tickAutohandle(deltaTime);

    ImGui::Begin("Main", static_cast<void*>(nullptr), ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar);
    auto* inst = static_cast<Instance*>(UImGui::Instance::getGlobal());

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

                YAML::Node o = inst->loadConfigGeneric();
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

                inst->outputConfig(o);
            }
            else if (ImGui::MenuItem("* Refresh"))
                inst->loadConfigData();
            else if (ImGui::MenuItem("+ Generate script"))
            {
                auto& pins = inst->selectedContainer->pins;
                for (auto& a : pins)
                    if (a.second)
                        inst->scriptGen.state = UIMGUI_COMPONENT_STATE_RUNNING;
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
                    {
                        if (IncusGetState(inst->selectedContainer->name.data()) == 0) // Powered off
                        {
                            INCUS_RUN(IncusStartContainer, inst->selectedContainer->name.data(), "power on");
                        }
                        IncusExec(inst->selectedContainer->name.data(), ("su{{b}}ubuntu{{b}}-c{{b}}" + pin.first + " & disown").data(), false);
                    }

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

void UntitledGameSystemManager::MainView::end() noexcept
{
    endAutohandle();

}