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
                            UImGui::FString name{};
                            c >> name;

                            if (name == inst->selectedContainer->name)
                            {
                                UImGui::TVector<UImGui::FString> arr{};
                                pins >> arr;

                                arr.push_back(cmd);

                                pins.clear_children();
                                pins << arr;
                                break;
                            }
                        }
                    }
                }
                inst->outputConfig(o);
                inst->selectedContainer->pins.push_back({ cmd, false });

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