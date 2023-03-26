#include "MainView.hpp"
#include "Instance.hpp"

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

    // TODO: In the original code, refreshPins was called here

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::MenuItem("+ New Pin"))
            inst->pin.state = UImGui::UIMGUI_COMPONENT_STATE_RUNNING;
        else if (ImGui::MenuItem("- Delete Pin"))
        {
            for (auto& pin : pins)
                if (pin.second)
                    ; // TODO: Call unpin here
        }
        else if (ImGui::MenuItem("* Refresh"))
            ; // TODO: Call refreshPins here
        else if (ImGui::MenuItem("+ Generate script"))
            for (auto& pin : pins)
                ; // TODO: Generate script for pins here, this includes desktop files, preferably with icons included

        ImGui::EndMenuBar();

        // This is just for keeping track of ids
        size_t i = 0;
        if (ImGui::BeginTable("table1", 2))
        {
            ImGui::TableNextColumn();
            for (auto& pin : pins)
            {
                ImGui::Checkbox(("##selected?" + std::to_string(i)).c_str(), &pin.second);
                ImGui::SameLine();
                ImGui::Text("%s", pin.first.c_str());
                ImGui::TableNextColumn();
                if (ImGui::ArrowButton(("##buttonPlay" + std::to_string(i)).c_str(), ImGuiDir_Right))
                {
                    std::cout << pin.first << std::endl;
                    // TODO: Play the pin
                }
                ImGui::TableNextColumn();
                i++;
            }

            ImGui::EndTable();
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

