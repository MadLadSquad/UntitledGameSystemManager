#include "About.hpp"

UntitledGameSystemManager::About::About()
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::About::begin()
{
    beginAutohandle();
    brandIcon.init(UIMGUI_CONTENT_DIR"brand-icon.png");
    brandIcon.loadImGui();

    lxcIcon.init(UIMGUI_CONTENT_DIR"linux-containers-icon.png");
    lxcIcon.loadImGui();
}

void UntitledGameSystemManager::About::tick(float deltaTime)
{
    tickAutohandle(deltaTime);
    if (!ImGui::IsPopupOpen("About us"))
        ImGui::OpenPopup("About us");
    if (ImGui::BeginPopupModal("About us", &state))
    {

        ImGui::TextWrapped("The UntitledGameSystemManager, a game system manager for Linux, developed and maintained by MadLad Squad");
        ImGui::TextWrapped("This project utilises lxc and lxd for creating and managing the containers, that are the core of this project!");
        ImGui::TextWrapped("More info about lxd and lxc: ");
        ImGui::SameLine();
        constexpr char* text = (char*)"https://linuxcontainers.org/";
        ImGui::InputText("##lxcinfo", text, strlen(text), ImGuiInputTextFlags_ReadOnly);

        ImGui::Image((void*)(intptr_t)lxcIcon.get(), { 50.0f, 50.0f });
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text("LXC/LXD logo");
            ImGui::EndTooltip();
        }
        ImGui::SameLine();
        ImGui::Image((void*)(intptr_t)brandIcon.get(), { 50.0f, 50.0f });
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text("MadLadSquad logo");
            ImGui::EndTooltip();
        }

        if (ImGui::Button("Close##Aboutus"))
            state = UIMGUI_COMPONENT_STATE_PAUSED;
        ImGui::EndPopup();
    }
}

void UntitledGameSystemManager::About::end()
{
    endAutohandle();

}

UntitledGameSystemManager::About::~About()
{

}