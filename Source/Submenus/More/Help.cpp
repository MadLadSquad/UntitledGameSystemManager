#include "Help.hpp"
#include "Instance.hpp"

UntitledGameSystemManager::Help::Help() noexcept
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::Help::begin() noexcept
{
    beginAutohandle();

}

void UntitledGameSystemManager::Help::tick(const float deltaTime) noexcept
{
    tickAutohandle(deltaTime);
    if (!ImGui::IsPopupOpen("Info"))
        ImGui::OpenPopup("Info");
    if (ImGui::BeginPopupModal("Info", &state))
    {
        const auto* inst = static_cast<Instance*>(UImGui::Instance::getGlobal());

        static UImGui::FString configDir = inst->configDir + "config/";
        static UImGui::FString scriptsDir = inst->configDir + "scripts/";
        static UImGui::FString resourcesDir = inst->configDir + "resources/";

        ImGui::Text("Config directories");

        ImGui::Text("Scripts");
        ImGui::SameLine();
        ImGui::InputText("##scriptsdir", &scriptsDir, ImGuiInputTextFlags_ReadOnly);

        ImGui::Text("Config");
        ImGui::SameLine();
        ImGui::InputText("##confdir", &configDir, ImGuiInputTextFlags_ReadOnly);

        ImGui::Text("Resources");
        ImGui::SameLine();
        ImGui::InputText("##resdir", &resourcesDir, ImGuiInputTextFlags_ReadOnly);


        if (ImGui::Button("Close##info"))
            state = UIMGUI_COMPONENT_STATE_PAUSED;
        ImGui::EndPopup();
    }
}

void UntitledGameSystemManager::Help::end() noexcept
{
    endAutohandle();

}