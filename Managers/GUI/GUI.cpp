#include "GUI.hpp"
#include <imgui.h>
#include <cpp/imgui_stdlib.h>
#include <pwd.h>
#include <yaml.h>

void UGM::Managers::GUI::render(UGM::GUI::Window& mainWindow)
{
    static bool bShowAboutUs = false;
    static bool bShowNew = false;
    static bool bShowRestart = false;
    static bool bShowPoweroff = false;
    static bool bShowPoweron = false;
    static bool bShowExit = false;
    static bool bShowDirectories = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Container"))
                bShowNew = true;

            if (ImGui::MenuItem("Show directories"))
                bShowDirectories = true;

            if (ImGui::MenuItem("Exit"))
                bShowExit = true;

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Poweroff", "all"))
                bShowPoweroff = true;

            if (ImGui::MenuItem("Poweron", "all"))
                bShowPoweron = true;

            if (ImGui::MenuItem("Restart", "all"))
                bShowRestart = true;
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("More"))
        {
            if (ImGui::MenuItem("About us"))
                bShowAboutUs = true;
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (bShowAboutUs)
        renderAboutUs(mainWindow, bShowAboutUs);
    if (bShowNew)
        renderNew(mainWindow, bShowNew);
    if (bShowRestart)
        renderRestart(mainWindow, bShowRestart);
    if (bShowPoweroff)
        renderPoweroff(mainWindow, bShowPoweroff);
    if (bShowPoweron)
        renderPoweron(mainWindow, bShowPoweron);
    if (bShowExit)
        renderExit(mainWindow, bShowExit);
    if (bShowDirectories)
        renderDirectories(mainWindow, bShowDirectories);
}

void UGM::Managers::GUI::renderAboutUs(UGM::GUI::Window& mainWindow, bool& bOpen)
{
    if (!ImGui::IsPopupOpen("About us"))
        ImGui::OpenPopup("About us");
    if (ImGui::BeginPopupModal("About us", &bOpen))
    {
        ImGui::TextWrapped("The UntitledLinuxGameManager, a game system manager for linux, developed and maintained by MadLad Squad");
        ImGui::TextWrapped("This project utilises lxc and lxd for creating and managing the containers, that are the core of the project!");
        ImGui::TextWrapped("More info about lxd and lxc: ");
        ImGui::SameLine();
        constexpr char* text = (char*)"https://linuxcontainers.org/";
        ImGui::InputText("##lxcinfo", text, strlen(text), ImGuiInputTextFlags_ReadOnly);

        //ImGui::Image((void*)(intptr_t)logo.getImage(), ImVec2((float)logo.getWidth() - 100, (float)logo.getHeight() - 100));

        if (ImGui::Button("Close##AboutUS"))
        {
            bOpen = false;
        }
        ImGui::EndPopup();
    }
}

std::string* UGM::Managers::GUI::renderSidebar(UGM::GUI::Window& mainWindow)
{
    static bool bFirst = true;
    static std::vector<std::string> containersList;
    static std::string* selectedContainer = nullptr;

    if (bFirst)
    {
        YAML::Node out;
        auto* passwd = getpwuid(geteuid());
        std::string file = std::string("/home/") + passwd->pw_name + "/.config/UntitledLinuxGameManager/config/layout.yaml";
        try
        {
            out = YAML::LoadFile(file);
        }
        catch (YAML::BadFile&)
        {
            logger.consoleLog("Couldn't open the config file!", UVK_LOG_TYPE_ERROR);
            return selectedContainer;
        }
        YAML::Node containers = out["containers"];
        if (containers)
            for (YAML::Node a : containers)
                if (a["container"] && a["pins"])
                    containersList.push_back(a["container"].as<std::string>());

        bFirst = false;
    }
    ImGui::Begin("Sidebar", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::MenuItem("+ New"))
            if (!ImGui::IsPopupOpen("New Warning"))
                ImGui::OpenPopup("New Warning");
        if (ImGui::MenuItem("* Refresh"))
        {
            containersList.clear();
            selectedContainer = nullptr;
            YAML::Node out;
            auto* passwd = getpwuid(geteuid());
            std::string file = std::string("/home/") + passwd->pw_name + "/.config/UntitledLinuxGameManager/config/layout.yaml";
            try
            {
                out = YAML::LoadFile(file);
            }
            catch (YAML::BadFile&)
            {
                logger.consoleLog("Couldn't open the config file!", UVK_LOG_TYPE_ERROR);
                return selectedContainer;
            }
            YAML::Node containers = out["containers"];
            if (containers)
                for (YAML::Node a : containers)
                    if (a["container"] && a["pins"])
                        containersList.push_back(a["container"].as<std::string>());
        }
        ImGui::EndMenuBar();
    }

    for (auto& a : containersList)
    {
        // In a click, a  user can change the state of the button, so to safely cleanup we save the outdated data
        bool bSelected = false;
        if (selectedContainer == &a)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.87f, 0.64f, 0.03, 1.0f });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f, 0.94f, 0.30f, 1.0f });
            bSelected = true;
        }
        else
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f, });


        if (ImGui::Button(a.c_str(), { ImGui::GetContentRegionAvail().x, 0 }))
        {
            selectedContainer = &a;
        }

        if (bSelected)
            ImGui::PopStyleColor(2);
        else
            ImGui::PopStyleColor();
    }
    ImGui::End();
    return selectedContainer;
}

void UGM::Managers::GUI::renderMainView(UGM::GUI::Window& mainWindow, std::string* selectedContainer, const bool& bReset)
{
    // a vector that keeps a pair of the name of the pinned application and a bool of weather it is selected
    static std::vector<std::pair<std::string, bool>> pins;
    if (bReset)
    {
        pins.clear();
        YAML::Node out;
        auto* passwd = getpwuid(geteuid());
        std::string file = std::string("/home/") + passwd->pw_name + "/.config/UntitledLinuxGameManager/config/layout.yaml";
        try
        {
            out = YAML::LoadFile(file);
        }
        catch (YAML::BadFile&)
        {
            logger.consoleLog("Couldn't open the config file!", UVK_LOG_TYPE_ERROR);
            return;
        }
        const YAML::Node& containers = out["containers"];
        if (containers)
            for (const YAML::Node& a : containers)
                if (a["container"] && a["pins"] && a["container"].as<std::string>() == *selectedContainer)
                    for (const YAML::Node& f : a["pins"])
                        pins.emplace_back( f.as<std::string>(), false );
    }
    else
    {
        ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar);
        if (selectedContainer != nullptr)
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::MenuItem("+ New Pin"));
                if (ImGui::MenuItem("- Delete Pin"));
                if (ImGui::MenuItem("* Refresh"))
                {
                    pins.clear();
                    YAML::Node out;
                    auto* passwd = getpwuid(geteuid());
                    std::string file = std::string("/home/") + passwd->pw_name + "/.config/UntitledLinuxGameManager/config/layout.yaml";
                    try
                    {
                        out = YAML::LoadFile(file);
                    }
                    catch (YAML::BadFile&)
                    {
                        logger.consoleLog("Couldn't open the config file!", UVK_LOG_TYPE_ERROR);
                        return;
                    }
                    const YAML::Node& containers = out["containers"];
                    if (containers)
                        for (const YAML::Node& a : containers)
                            if (a["container"] && a["pins"] && a["container"].as<std::string>() == *selectedContainer)
                                for (const YAML::Node& f : a["pins"])
                                    pins.emplace_back( f.as<std::string>(), false );
                }
                if (ImGui::MenuItem("+ Generate Script"));
                ImGui::EndMenuBar();
            }
            size_t i = 0;
            for (auto& ff : pins)
            {
                ImGui::Columns(2, nullptr, false);
                ImGui::Checkbox(("##Selected?" + std::to_string(i)).c_str(), &ff.second);
                ImGui::SameLine();
                ImGui::Text("%s", ff.first.c_str());
                ImGui::NextColumn();
                ImGui::ArrowButton(("##ButtonPlay" + std::to_string(i)).c_str(), ImGuiDir_Right);
                ImGui::Columns(1, nullptr, false);
                i++;
            }
        }
        ImGui::End();
    }
}

void UGM::Managers::GUI::renderWindows(UGM::GUI::Window& mainWindow)
{
    // Will render the sidebar, and pass its selected container to the main view
    // The whole selected container passing here is like having a weirder type of singleton lmao
    renderMainView(mainWindow, renderSidebar(mainWindow));
}

void UGM::Managers::GUI::renderNew(UGM::GUI::Window& mainWindow, bool& bOpen)
{
    if (!ImGui::IsPopupOpen("New Warning"))
        ImGui::OpenPopup("New Warning");
    if (ImGui::BeginPopupModal("Restart Warning", &bOpen))
    {
        ImGui::TextWrapped("This will launch the UntitledLinuxGameManager GUI installer!");

        if (ImGui::Button("Cancel##poweroff"))
            bOpen = false;
        ImGui::SameLine();
        if (ImGui::Button("Start##new"))
        {
            bOpen = false;
            mainWindow.closeWindow();
        }
        ImGui::EndPopup();
    }
}

void UGM::Managers::GUI::renderRestart(UGM::GUI::Window& mainWindow, bool& bOpen)
{
    if (!ImGui::IsPopupOpen("Restart Warning"))
        ImGui::OpenPopup("Restart Warning");
    if (ImGui::BeginPopupModal("Restart Warning", &bOpen))
    {
        ImGui::TextWrapped("This will restart all the containers currently stopped! This may take a couple of minutes!");

        if (ImGui::Button("Cancel##poweroff"))
            bOpen = false;
        ImGui::SameLine();
        if (ImGui::Button("Restart##restart"))
        {
            bOpen = false;
            mainWindow.closeWindow();
        }
        ImGui::EndPopup();
    }
}

void UGM::Managers::GUI::renderPoweroff(UGM::GUI::Window& mainWindow, bool& bOpen)
{
    if (!ImGui::IsPopupOpen("Poweroff Warning"))
        ImGui::OpenPopup("Poweroff Warning");
    if (ImGui::BeginPopupModal("Poweroff Warning", &bOpen))
    {
        ImGui::TextWrapped("This will power off all the containers currently stopped! This may take a couple of minutes!");

        if (ImGui::Button("Cancel##poweroff"))
            bOpen = false;
        ImGui::SameLine();
        if (ImGui::Button("Poweroff##poweroff"))
        {
            bOpen = false;
            mainWindow.closeWindow();
        }
        ImGui::EndPopup();
    }
}

void UGM::Managers::GUI::renderPoweron(UGM::GUI::Window& mainWindow, bool& bOpen)
{
    if (!ImGui::IsPopupOpen("Poweron Warning"))
        ImGui::OpenPopup("Poweron Warning");
    if (ImGui::BeginPopupModal("Poweron Warning", &bOpen))
    {
        ImGui::TextWrapped("This will power on all the containers currently stopped! This may take a couple of minutes!");

        if (ImGui::Button("Cancel##poweron"))
            bOpen = false;
        ImGui::SameLine();
        if (ImGui::Button("Poweron##poweron"))
        {
            bOpen = false;
            mainWindow.closeWindow();
        }
        ImGui::EndPopup();
    }
}

void UGM::Managers::GUI::renderExit(UGM::GUI::Window& mainWindow, bool& bOpen)
{
    if (!ImGui::IsPopupOpen("Exit warning"))
        ImGui::OpenPopup("Exit warning");
    if (ImGui::BeginPopupModal("Exit warning", &bOpen))
    {
        ImGui::TextWrapped("You're exiting the UntitledLinuxGameManager! All of the applications or containers already opened will remain opened!");

        if (ImGui::Button("Cancel##exit"))
            bOpen = false;
        ImGui::SameLine();
        if (ImGui::Button("Exit##exit"))
        {
            bOpen = false;
            mainWindow.closeWindow();
        }
        ImGui::EndPopup();
    }
}

void UGM::Managers::GUI::renderDirectories(UGM::GUI::Window& mainWindow, bool& bOpen)
{
    if (!ImGui::IsPopupOpen("Show Directories"))
        ImGui::OpenPopup("Show Directories");
    if (ImGui::BeginPopupModal("Show Directories", &bOpen))
    {
        static std::string configDir = std::string("/home") + getpwuid(geteuid())->pw_name + "/.config/UntitledLinuxGameManager/config";
        static std::string scriptsDir = std::string("/home") + getpwuid(geteuid())->pw_name + "/.config/UntitledLinuxGameManager/scripts";
        static std::string resourcesDir = std::string("/home") + getpwuid(geteuid())->pw_name + "/.config/UntitledLinuxGameManager/resources";

        ImGui::Text("Scripts");
        ImGui::SameLine();
        ImGui::InputText("##scriptsdir", &scriptsDir, ImGuiInputTextFlags_ReadOnly);

        ImGui::Text("Config");
        ImGui::SameLine();
        ImGui::InputText("##confdir", &configDir, ImGuiInputTextFlags_ReadOnly);

        ImGui::Text("Resources");
        ImGui::SameLine();
        ImGui::InputText("##resdir", &resourcesDir, ImGuiInputTextFlags_ReadOnly);

        if (ImGui::Button("Exit##dirs"))
            bOpen = false;
        ImGui::EndPopup();
    }
}