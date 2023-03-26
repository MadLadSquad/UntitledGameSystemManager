#include "Help.hpp"
#include <unistd.h>
#include <pwd.h>

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
        static std::string configDir = std::string("/home") + getpwuid(geteuid())->pw_name + "/.config/UntitledLinuxGameManager/config/";
        static std::string scriptsDir = std::string("/home") + getpwuid(geteuid())->pw_name + "/.config/UntitledLinuxGameManager/scripts/";
        static std::string resourcesDir = std::string("/home") + getpwuid(geteuid())->pw_name + "/.config/UntitledLinuxGameManager/resources/";

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

