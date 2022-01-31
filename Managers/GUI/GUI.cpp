#include "GUI.hpp"
#include <imgui.h>
#include <cpp/imgui_stdlib.h>
#include <pwd.h>
#include <yaml.h>
#include "Commands.hpp"

void UGM::Managers::GUI::render(UGM::GUI::Window& mainWindow)
{
    static bool bShowAboutUs = false;
    static bool bShowExit = false;
    static bool bShowDirectories = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Container"))
                global.bShowNew = true;

            if (ImGui::MenuItem("Show directories"))
                bShowDirectories = true;

            if (ImGui::MenuItem("Exit"))
                bShowExit = true;

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Power off"))
                global.bShowPoweroff = true;
            if (ImGui::MenuItem("Power on"))
                global.bShowPoweron = true;
            if (ImGui::MenuItem("Restart"))
                global.bShowRestart = true;
            if (ImGui::MenuItem("New pin"))
                global.bShowPin = true;
            if (ImGui::MenuItem("Update"))
                global.bShowUpdate = true;
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
    if (global.bShowNew)
        renderNew(mainWindow, global.bShowNew);
    if (global.bShowRestart)
        renderRestart(mainWindow, global.bShowRestart);
    if (global.bShowPoweroff)
        renderPoweroff(mainWindow, global.bShowPoweroff);
    if (global.bShowPoweron)
        renderPoweron(mainWindow, global.bShowPoweron);
    if (bShowExit)
        renderExit(mainWindow, bShowExit);
    if (bShowDirectories)
        renderDirectories(mainWindow, bShowDirectories);
    if (global.bShowPin)
        renderPin(mainWindow, global.bShowPin);
    if (global.bShowDelete)
        renderDelete(mainWindow, global.bShowDelete);
    if (global.bShowUpdate)
        renderUpdate(mainWindow, global.bShowUpdate);
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
            global.bShowNew = true;
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
        if (ImGui::MenuItem("- Delete"))
            global.bShowDelete = true;
        if (ImGui::BeginMenu("* Power"))
        {
            if (ImGui::MenuItem("* Power on"))
                global.bShowPoweron = true;
            if (ImGui::MenuItem("* Power off"))
                global.bShowPoweroff = true;
            if (ImGui::MenuItem("* Restart"))
                global.bShowRestart = true;
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("* Update"))
            global.bShowUpdate = true;

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
        refreshPins(pins);
    else
    {
        ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar);
        if (selectedContainer != nullptr)
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::MenuItem("+ New Pin"))
                    global.bShowPin = true;
                if (ImGui::MenuItem("- Delete Pin"))
                {
                    for (auto& p : pins)
                        if (p.second)
                            unpin(const_cast<char*>(selectedContainer->c_str()), const_cast<char*>(p.first.c_str()));
                    refreshPins(pins);
                }

                if (ImGui::MenuItem("* Refresh"))
                    refreshPins(pins);
                if (ImGui::MenuItem("+ Generate Script"))
                    for (auto& p : pins)
                        genscript(const_cast<char*>(selectedContainer->c_str()), const_cast<char*>(p.first.c_str()));
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
                if (ImGui::ArrowButton(("##ButtonPlay" + std::to_string(i)).c_str(), ImGuiDir_Right))
                {
                    // construct a really large string to run our command
                    std::string cmd = (std::string("lxc exec ") + *selectedContainer + " -- bash -c \"su ubuntu -c '" + std::string(ff.first) + "'\" &> /tmp/" + ff.first + ".log & disown");
                    // Ugly ass const cast right here
                    char* const args[] = { (char*)"bash", (char*)"-c", const_cast<char*>(cmd.c_str()), nullptr };
                    UGM::Core::Utilities::execandwait(args);
                }
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
    global.selectedContainerG = renderSidebar(mainWindow);
    renderMainView(mainWindow, global.selectedContainerG);
}

void UGM::Managers::GUI::renderNew(UGM::GUI::Window& mainWindow, bool& bOpen)
{
    if (!ImGui::IsPopupOpen("New Warning"))
        ImGui::OpenPopup("New Warning");
    if (ImGui::BeginPopupModal("New Warning", &bOpen))
    {
        ImGui::TextWrapped("This will launch the UntitledLinuxGameManager GUI installer!");

        if (ImGui::Button("Cancel##nnew"))
            bOpen = false;
        ImGui::SameLine();
        if (ImGui::Button("Start##new"))
        {
            bOpen = false;
            newContainer();
        }
        ImGui::EndPopup();
    }
}

void UGM::Managers::GUI::renderRestart(UGM::GUI::Window& mainWindow, bool& bOpen)
{
    if (!ImGui::IsPopupOpen("Restart Warning"))
        ImGui::OpenPopup("Restart Warning");
    if (ImGui::BeginPopupModal("Restart Warning"))
    {
        static bool bStarted = false;
        static UGM::Core::Utilities::ScriptRunner runner;

        ImGui::TextWrapped("This will restart the currently selected container! This may take a couple of minutes!");

        if (!bStarted)
        {
            if (ImGui::Button("Cancel##restart"))
                bOpen = false;
            ImGui::SameLine();
            if (ImGui::Button("Restart##restart"))
            {
                char* const args[] = { (char*)"lxc", (char*)"restart", const_cast<char*>(global.selectedContainerG->c_str()), nullptr };
                runner.init(args);

                bStarted = true;
            }
        }
        else
        {
            if (UGM::Core::Utilities::currentpid == -1)
            {
                runner.destroyForReuse();
                bOpen = false;
            }
        }
        ImGui::EndPopup();
    }
}

void UGM::Managers::GUI::renderPoweroff(UGM::GUI::Window& mainWindow, bool& bOpen)
{
    if (!ImGui::IsPopupOpen("Power off Warning"))
        ImGui::OpenPopup("Power off Warning");
    if (ImGui::BeginPopupModal("Power off Warning"))
    {
        static bool bStarted = false;
        static UGM::Core::Utilities::ScriptRunner runner;

        ImGui::TextWrapped("This will power off the currently selected container! This may take a couple of minutes!");

        if (!bStarted)
        {
            if (ImGui::Button("Cancel##poweroff"))
                bOpen = false;
            ImGui::SameLine();
            if (ImGui::Button("Power off##poweroff"))
            {
                char* const args[] = { (char*)"lxc", (char*)"stop", const_cast<char*>(global.selectedContainerG->c_str()), nullptr };
                runner.init(args);

                bStarted = true;
            }
        }
        else
        {
            if (UGM::Core::Utilities::currentpid == -1)
            {
                runner.destroyForReuse();
                bOpen = false;
            }
        }
        ImGui::EndPopup();
    }
}

void UGM::Managers::GUI::renderPoweron(UGM::GUI::Window& mainWindow, bool& bOpen)
{
    if (!ImGui::IsPopupOpen("Power on Warning"))
        ImGui::OpenPopup("Power on Warning");
    if (ImGui::BeginPopupModal("Power on Warning"))
    {
        static bool bStarted = false;
        static UGM::Core::Utilities::ScriptRunner runner;

        ImGui::TextWrapped("This will power on the currently selected container! This may take a couple of minutes!");

        if (!bStarted)
        {
            if (ImGui::Button("Cancel##poweron"))
                bOpen = false;
            ImGui::SameLine();
            if (ImGui::Button("Power on##poweron") && global.selectedContainerG != nullptr)
            {
                char* const args[] = { (char*)"lxc", (char*)"start", const_cast<char*>(global.selectedContainerG->c_str()), nullptr };
                runner.init(args);

                bStarted = true;
            }
        }
        else
        {
            if (UGM::Core::Utilities::currentpid == -1)
            {
                runner.destroyForReuse();
                bOpen = false;
            }
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

void UGM::Managers::GUI::renderPin(UGM::GUI::Window& mainWindow, bool& bOpen)
{
    static std::string pinname;
    if (!ImGui::IsPopupOpen("Pin"))
        ImGui::OpenPopup("Pin");
    if (ImGui::BeginPopupModal("Pin"))
    {
        ImGui::Text("Type your desired command");
        ImGui::SameLine();
        ImGui::InputText("##pinInput", &pinname);

        if (ImGui::Button("Exit##pin"))
            bOpen = false;
        ImGui::SameLine();
        if (ImGui::Button("Pin##pin") && global.selectedContainerG != nullptr)
        {
            pin(const_cast<char*>(global.selectedContainerG->c_str()), const_cast<char*>(pinname.c_str()));
            bOpen = false;
        }
        ImGui::EndPopup();
    }
}

void UGM::Managers::GUI::refreshPins(std::vector<std::pair<std::string, bool>>& pins)
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
            if (global.selectedContainerG != nullptr && a["container"] && a["pins"] && a["container"].as<std::string>() == *global.selectedContainerG)
                for (const YAML::Node& f : a["pins"])
                    pins.emplace_back( f.as<std::string>(), false );
}

void UGM::Managers::GUI::renderDelete(UGM::GUI::Window& mainWindow, bool& bOpen)
{
    if (!ImGui::IsPopupOpen("Delete"))
        ImGui::OpenPopup("Delete");
    if (ImGui::BeginPopupModal("Delete"))
    {
        static bool bStartedDeleting = false;
        static UGM::Core::Utilities::ScriptRunner runner;

        if (global.selectedContainerG != nullptr)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, {1.0f, 0.0f, 0.0f, 1.0f});
            ImGui::TextWrapped("You're trying to delete %s! Once you press \"Delete\" this action cannot be reversed!", global.selectedContainerG->c_str());
            ImGui::PopStyleColor();
        }
        else
            ImGui::TextWrapped("You haven't selected a container!");

        if (bStartedDeleting)
            ImGui::TextWrapped("Container deletion started, this popup should automatically close when its operation is complete!");
        if (!bStartedDeleting && ImGui::Button("Exit##delete"))
            bOpen = false;
        if (!bStartedDeleting && global.selectedContainerG != nullptr)
        {
            ImGui::SameLine();
            if (ImGui::Button("Delete##cont") && !global.selectedContainerG->empty())
            {
                char* const args[] = { (char*)"lxc", (char*)"delete", const_cast<char*>(global.selectedContainerG->c_str()), nullptr };
                runner.init(args);
                bStartedDeleting = true;
            }
        }

        if (bStartedDeleting && UGM::Core::Utilities::currentpid == -1)
        {
            bOpen = false;
            bStartedDeleting = false;
            runner.destroyForReuse();
            auto* passwd = getpwuid(geteuid());
            std::string file = std::string("/home/") + passwd->pw_name + "/.config/UntitledLinuxGameManager/config/layout.yaml";
            std::string buf;
            std::string buffer;

            std::ifstream inf(file);
            bool bErase = false;
            while (std::getline(inf, buf))
            {
                if (buf.find(std::string("- container: ") + *global.selectedContainerG) != std::string::npos)
                {
                    bErase = true;
                    continue;
                }

                if (bErase)
                {
                    if (buf.find("- container: ") != std::string::npos)
                        bErase = false;
                }
                else
                    buffer += buf + '\n';
            }

            std::ofstream fout(file);
            fout << buffer << std::endl;
            fout.close();
            global.selectedContainerG = nullptr;
        }

        ImGui::EndPopup();
    }
}

void UGM::Managers::GUI::renderUpdate(UGM::GUI::Window& mainWindow, bool& bOpen)
{
    static UGM::Core::Utilities::ScriptRunner runner;
    static bool bDoDriverUpdate = false;
    static std::string comboVal;
    static char drvtype[2] = "M";
    static bool bStartedUpdate = false;
    static UGM::Core::Utilities::ScriptRunner scriptRunner;
    if (!ImGui::IsPopupOpen("Update##System"))
        ImGui::OpenPopup("Update##System");
    if (ImGui::BeginPopupModal("Update##System"))
    {
        ImGui::Text("Do you want to update your GPU drivers?");
        ImGui::SameLine();
        ImGui::Checkbox("##drvupdate", &bDoDriverUpdate);

        ImGui::Text("Select your GPU driver type");
        ImGui::SameLine();
        if (ImGui::BeginCombo("##DriverTP", comboVal.c_str()))
        {
            if (ImGui::MenuItem("NVidia##2"))
            {
                comboVal = "NVidia";
                drvtype[0] = 'N';
            }

            if (ImGui::MenuItem("Mesa(AMD/Intel)##2"))
            {
                comboVal = "Mesa";
                drvtype[0] = 'M';
            }
            ImGui::EndCombo();
        }

        if (!bStartedUpdate)
        {
            if (ImGui::Button("Exit##update"))
                bOpen = false;
            ImGui::SameLine();
            if (ImGui::Button("Start##update"))
            {
                std::string path = std::string("/home/") + getpwuid(geteuid())->pw_name + "/.config/UntitledLinuxGameManager/scripts/ugm-cli-update.sh";
                if (bDoDriverUpdate)
                {
                    char* const args[] = { const_cast<char*>(path.c_str()), (char*)"--name", const_cast<char*>(global.selectedContainerG->c_str()), (char*)"--driver", drvtype, (char*)"--gpu", nullptr };
                    runner.init(args);
                }
                else
                {
                    char* const args[] = { const_cast<char*>(path.c_str()), (char*)"--name", const_cast<char*>(global.selectedContainerG->c_str()), (char*)"--driver", drvtype, nullptr };
                    runner.init(args);
                }
                bStartedUpdate = true;
            }
        }
        else
        {
            if (UGM::Core::Utilities::currentpid == -1)
            {
                runner.destroyForReuse();
                bOpen = false;
                bStartedUpdate = false;
                bDoDriverUpdate = false;
            }
        }

        ImGui::EndPopup();
    }
}