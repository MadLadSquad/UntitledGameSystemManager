#include "MainView.hpp"

#include <ranges>
#include "Instance.hpp"
#include "yaml-cpp/yaml.h"

UntitledLinuxGameManager::MainView::MainView()
{

}

void UntitledLinuxGameManager::MainView::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::MainView::tick(float deltaTime)
{
    tickAutohandle(deltaTime);

    ImGui::Begin("Main", (void*)nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar);
    auto* inst = (Instance*)UImGui::Instance::getGlobal();

    if (inst->selectedContainer != nullptr)
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::MenuItem("+ New Pin"))
                inst->pin.state = UImGui::UIMGUI_COMPONENT_STATE_RUNNING;
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

                    if (ImGui::ArrowButton("##buttonPlay", ImGuiDir_Right))
                    {
                        std::cout << pin.first << std::endl;
                        // TODO: Play the pin
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

void UntitledLinuxGameManager::MainView::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::MainView::~MainView()
{

}