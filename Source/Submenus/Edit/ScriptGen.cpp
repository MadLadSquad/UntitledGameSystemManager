#include "ScriptGen.hpp"
#include "Instance.hpp"

UntitledGameSystemManager::ScriptGen::ScriptGen() noexcept
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::ScriptGen::begin() noexcept
{
    beginAutohandle();

}

void UntitledGameSystemManager::ScriptGen::tick(const float deltaTime) noexcept
{
    tickAutohandle(deltaTime);
    const auto* inst = static_cast<Instance*>(UImGui::Instance::getGlobal());
    if (inst->selectedContainer != nullptr)
    {
        if (!ImGui::IsPopupOpen("Generate script"))
            ImGui::OpenPopup("Generate script");
        if (ImGui::BeginPopupModal("Generate script", &state))
        {
            ImGui::TextWrapped("This menu creates a custom launch script to for all selected pinned applications! Add "
                               "\"$XDG_CONFIG_HOME/UntitledGameSystemManager/scripts\" or \"~/.config/UntitledGameSystemManager/scripts\""
                               "to your \"PATH\" environment variable to access them from the CLI.");

            ImGui::Text("The following scripts will be generated: ");
            for (auto& a : inst->selectedContainer->pins)
                if (a.second)
                    ImGui::BulletText("%s-%s", inst->selectedContainer->name.c_str(), a.first.c_str());

            if (ImGui::Button("Create"))
            {
                for (auto& a : inst->selectedContainer->pins)
                    if (a.second)
                        generateScript(a.first);
                state = UIMGUI_COMPONENT_STATE_PAUSED;
            }
            ImGui::SameLine();
            if (ImGui::Button("Close##info"))
                state = UIMGUI_COMPONENT_STATE_PAUSED;
            ImGui::EndPopup();
        }
    }
}

void UntitledGameSystemManager::ScriptGen::end() noexcept
{
    endAutohandle();

}

void UntitledGameSystemManager::ScriptGen::generateScript(const UImGui::FString& command) noexcept
{
    const auto* inst = static_cast<Instance*>(UImGui::Instance::getGlobal());
    const std::filesystem::path p(inst->configDir + "scripts/" + inst->selectedContainer->name + "-" + command);
    std::ofstream out(p);
    out << "#!/bin/bash" << std::endl << "lxc exec " << inst->selectedContainer->name << " -- bash -c '" << command << "'" << std::endl;

    permissions(p, std::filesystem::perms::owner_exec, std::filesystem::perm_options::add);
}
