#include "Help.hpp"
#include "Instance.hpp"

UntitledLinuxGameManager::Help::Help()
{
    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledLinuxGameManager::Help::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::Help::tick(float deltaTime)
{
    tickAutohandle(deltaTime);
    if (!ImGui::IsPopupOpen("Info"))
        ImGui::OpenPopup("Info");
    if (ImGui::BeginPopupModal("Info", &state))
    {
        auto* inst = (Instance*)UImGui::Instance::getGlobal();

        static std::string configDir = inst->configDir + "config/";
        static std::string scriptsDir = inst->configDir + "scripts/";
        static std::string resourcesDir = inst->configDir + "resources/";

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
            state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
        ImGui::EndPopup();
    }
}

void UntitledLinuxGameManager::Help::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::Help::~Help()
{

}

