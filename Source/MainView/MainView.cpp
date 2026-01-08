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

                ryml::Tree tree{};
                auto o = inst->loadConfigGeneric(tree);

                auto cont = o["containers"];
                if (keyValid(cont) && cont.is_seq())
                {
                    cont.clear_style();
                    for (auto a : cont.children())
                    {
                        auto n = a["container"];
                        auto p = a["pins"];

                        if (keyValid(n) && keyValid(p) && p.is_seq())
                        {
                            UImGui::FString str{};
                            n >> str;

                            if (str == inst->selectedContainer->name)
                            {
                                p.clear_children();

                                for (const auto& s : pins)
                                    p.append_child() << s.first;
                                break;
                            }
                        }
                    }
                }
                inst->outputConfig(o);
            }
            else if (ImGui::MenuItem("* Refresh"))
            {
                const auto name = inst->selectedContainer->name;
                inst->loadConfigData();

                // Since loadConfigData sets the selected container to null, we can automatically select the previously
                // selected container again for a smooth refresh
                bool bFound = false;
                for (auto& c : inst->containers)
                {
                    if (c.name == name)
                    {
                        inst->selectedContainer = &c;
                        bFound = true;
                        break;
                    }
                }

                // If for some reason the config doesn't contain the current container end early
                if (!bFound)
                {
                    ImGui::EndMenuBar();
                    ImGui::End();
                    return;
                }
            }
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
            if (inst->selectedContainer != nullptr && ImGui::BeginTable("MainTable", 2))
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
                            IncusStartContainer(inst->selectedContainer->name.data());
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