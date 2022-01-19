#include "GUI.hpp"
#include <imgui.h>
#include <cpp/imgui_stdlib.h>
#include <Core/../../GUI/Window.hpp>
#include <Core/Utilities.hpp>
#include <pwd.h>

void UGM::Installer::GUI::render(UGM::GUI::Window& mainWindow)
{
    static std::string user;
    static uint8_t currentSlide = 0;

    switch (currentSlide)
    {
    case 0:
        slide0(mainWindow, currentSlide);
        break;
    case 1:
        user = slide1(mainWindow, currentSlide);
        break;
    case 2:
        slide2(user, mainWindow, currentSlide);
        break;
    case 3:
        break;
    default:
        currentSlide = 0;
    }
}

void UGM::Installer::GUI::slide0(UGM::GUI::Window& mainWindow, uint8_t& currentSlide) noexcept
{
    ImGui::TextWrapped("Welcome to the Untitled Linux Game Manager GUI installer!");
    ImGui::TextWrapped("This installer will automatically install everything for your new Ubuntu gaming container!");
    if (ImGui::Button("Exit"))
        mainWindow.closeWindow();
    ImGui::SameLine();
    if (ImGui::Button("Start setup"))
        currentSlide++;
    ImGui::SameLine();
    if (ImGui::Button("Skip first time setup!"))
        currentSlide += 2;
}

const std::string& UGM::Installer::GUI::slide1(UGM::GUI::Window& mainWindow, uint8_t& currentSlide, bool cleanup) noexcept
{
    static std::string username = "root";
    static bool bEnabled = false; // Used by the confirmation checkbox
    static bool bFirst = true; // Execute on the first function call
    static bool bFirstTimeRunning = true; // Used for switching which script to update, if it is true then update the user script, otherwise do nothing
    static bool bStartedScript = false; // Signal that we started running the installation script
    static bool bFinished = false; // Signal that the execution of the installRunner has finished and the UI can be changed
    static Core::Utilities::ScriptRunner userRunner; // Will run a command to get all the users
    static Core::Utilities::ScriptRunner installRunner; // Will actually run the installation script
    static std::thread scriptRunOffloader; // This thread will exclusively work on updating the text from the log of the installRunner
    static bool bAlreadyJoined = false; // Signal if the thread was already joined before
    static bool bUsedThread = false; // Signal if the thread was used since we can skip this slide!

    if (cleanup)
    {
        if (!bAlreadyJoined && bUsedThread)
        {
            scriptRunOffloader.join();
            bAlreadyJoined = true;
        }
        else
            logger.consoleLog("Thread already joined, exiting!", UVK_LOG_TYPE_SUCCESS);
        return username;
    }

    if (bFirst)
    {
        constexpr char* const args[] = { (char*)"awk", (char*)"-F:", (char*)"{ print $1}", (char*)"/etc/passwd", nullptr };
        userRunner.init(args);
        userRunner.updateBufferSize();
        userRunner.update(true);
        bFirst = false;
    }
    if (bFirstTimeRunning)
    {
        userRunner.update();
        userRunner.updateBufferSize();
    }

    ImGui::TextWrapped("Select your regular user");
    ImGui::SameLine();
    if (ImGui::BeginCombo("##ComboUserIn", username.c_str()))
    {
        for (auto& a : userRunner.data())
            if (ImGui::MenuItem(a.c_str()))
                username = a;
        ImGui::EndCombo();
    }

    ImGui::PushStyleColor(ImGuiCol_Text, { 1.0, 0.0, 0.0, 1.0 });
    ImGui::TextWrapped("Before we begin make sure that LXD and LXC are installed on your system!");
    ImGui::PopStyleColor();

    ImGui::TextWrapped("Yes I do have LXD and LXC installed on my system and I am ready to proceed!");
    ImGui::SameLine();
    ImGui::Checkbox("##Agree with lxd/lxc", &bEnabled);

    if (bFinished)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 1.0f, 0.0f, 1.0f });
        ImGui::TextWrapped("Install finished, press \"Next\" to proceed!");
        ImGui::PopStyleColor();
    }

    if (ImGui::Button("Exit"))
        mainWindow.closeWindow();
    if (!bFinished)
    {
        if (!bStartedScript)
        {
            ImGui::SameLine();
            if (ImGui::Button("Start") && bEnabled && bFirstTimeRunning)
            {
                bStartedScript = true;
                std::string file = std::string("/home/") + username + "/.config/UntitledLinuxGameManager/scripts/ugm-cli-prepare-install.sh";

                char* const args[] = { const_cast<char*>(file.c_str()), (char*)"--name", const_cast<char*>(username.c_str()), nullptr };
                installRunner.init(args);
                installRunner.updateBufferSize();
                installRunner.update(true);
                scriptRunOffloader = std::thread([&](){
                    while (installRunner.valid() && !mainWindow.getWindowClose())
                    {
                        installRunner.updateBufferSize();
                        installRunner.update();
                    }
                });
                bUsedThread = true;
                bFirstTimeRunning = false;
            }
        }
    }
    else
    {
        ImGui::SameLine();
        if (ImGui::Button("Next"))
        {
            currentSlide++;
            slide1(mainWindow, currentSlide, true);
            return username;
        }
    }

    if (bStartedScript)
    {
        ImGui::Separator();
        ImGui::BeginChild("##MessageLog", { 0, 0 }, false, ImGuiWindowFlags_NoScrollbar);
        for (auto& a : installRunner.data())
            ImGui::Text("%s", a.c_str());
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
        ImGui::EndChild();
        if (Core::Utilities::currentpid == -1)
        {
            installRunner.destroy();
            bFinished = true;
        }
    }
    return username;
}

void UGM::Installer::GUI::slide2(std::string& user, UGM::GUI::Window& mainWindow, uint8_t& currentSlide, bool cleanup) noexcept
{
    static bool bStartSetup = false;
    static bool bFinished = false;
    static bool bStartedExecuting = false;
    static char driverType[2] = "0";
    static std::string comboText = "none";
    static std::string containerName;

    static UGM::Core::Utilities::ScriptRunner runner;
    static std::thread offloadThread;
    static bool bJoined = false;
    static bool bUsedThread = false;

    if (cleanup)
    {
        if (!bJoined && bUsedThread)
        {
            offloadThread.join();
            bJoined = true;
        }
        else
            logger.consoleLog("Thread already joined, exiting!", UVK_LOG_TYPE_SUCCESS);
        return;
    }

    ImGui::TextWrapped("Welcome to the container creation wizard, here you can setup your new container!");
    ImGui::Text("Choose your GPU device type");
    ImGui::SameLine();
    if (ImGui::BeginCombo("##ComboDrivers", comboText.c_str()))
    {
        if (ImGui::MenuItem("NVidia"))
        {
            driverType[0] = 'N';
            comboText = "NVidia";
        }

        if (ImGui::MenuItem("Mesa(AMD/Intel)"))
        {
            driverType[0] = 'M';
            comboText = "Mesa(AMD/Intel)";
        }
        ImGui::EndCombo();
    }

    ImGui::Text("Enter in your container's name!");
    ImGui::SameLine();
    ImGui::InputText("##ContainerName", &containerName);

    ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.0f, 0.0f, 1.0f });
    ImGui::TextWrapped("The setup may take longer than 30 minutes, make sure that you have a stable internet connection and that the LXD daemon is running without problems! The installer may not respond for around 1-2 minutes!");
    ImGui::PopStyleColor();

    ImGui::Text("Start setup");
    ImGui::SameLine();
    ImGui::Checkbox("##checkboxstart", &bStartSetup);

    if (bFinished)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 1.0f, 0.0f, 1.0f });
        ImGui::TextWrapped("Installation of the container finished, you can now exit this installer!");
        ImGui::PopStyleColor();
    }

    if (ImGui::Button("Exit"))
        mainWindow.closeWindow();

    if (!bFinished)
    {
        if (!bStartedExecuting)
        {
            ImGui::SameLine();
            if (ImGui::Button("Install"))
            {
                if (user.empty() || user == "root")
                    user = std::string(getpwuid(geteuid())->pw_name);
                std::string file = std::string("/home/") + user + "/.config/UntitledLinuxGameManager/scripts/ugm-cli-install.sh";
                std::string execStr = file + " --name " + containerName + " --driver " + driverType;
                char* const launch[] = { (char*)"lxc", (char*)"launch", (char*)"images:archlinux", const_cast<char*>(containerName.c_str()), nullptr };
                UGM::Core::Utilities::execandwait(launch);
                char* const args[] = { (char*)"bash", (char*)"-c", const_cast<char*>(execStr.c_str()), nullptr };

                runner.init(args);
                runner.updateBufferSize();
                runner.update(true);
                offloadThread = std::thread([&](){
                    while (runner.valid() && !mainWindow.getWindowClose())
                    {
                        runner.updateBufferSize();
                        runner.update();
                    }
                });
                bStartedExecuting = true;
                bUsedThread = true;
            }
        }

    }
    if (bStartedExecuting)
    {
        ImGui::Separator();
        ImGui::BeginChild("##MessageLog", { 0.0f, 0.0f }, false, ImGuiWindowFlags_NoScrollbar);
        for (auto& a : runner.data())
            ImGui::Text("%s", a.c_str());
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
        ImGui::EndChild();
        if (Core::Utilities::currentpid == -1)
        {
            runner.destroy();
            bFinished = true;
        }
    }
}