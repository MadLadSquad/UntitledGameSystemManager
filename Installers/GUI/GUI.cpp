#include "GUI.hpp"
#include <imgui.h>
#include <cpp/imgui_stdlib.h>
#include <Core/../../GUI/Window.hpp>
#include <Core/Utilities.hpp>
#include <sys/wait.h>

void UGM::Installer::GUI::render(UGM::GUI::Window& mainWindow)
{
    static uint8_t currentSlide = 0;

    switch (currentSlide)
    {
    case 0:
        slide0(mainWindow, currentSlide);
        break;
    case 1:
        slide1(mainWindow, currentSlide);
        break;
    case 2:
        slide2(mainWindow, currentSlide);
        break;
    case 3:
        slide3(mainWindow, currentSlide);
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

void UGM::Installer::GUI::slide1(UGM::GUI::Window& mainWindow, uint8_t& currentSlide) noexcept
{
    static std::string username = "root";
    //static std::vector<std::string> usernamesBuffer;
    static bool bEnabled = false;
    static bool bFirst = true;
    static bool bFirstTimeRunning = true;
    //static std::thread tr;
    //static pid_t pid;
    //static std::vector<std::string> messageBuffer;
    static bool bStartedScript = false;
    //static std::thread scriptThread;
    //static pid_t scriptpid;
    static Core::Utilities::ScriptRunner userRunner;
    static Core::Utilities::ScriptRunner installRunner;
    static uint8_t framesPassed = 0; // Used to ratelimit calls to update
    static std::thread scriptRunOffloader;

    framesPassed++;
    if (bFirst)
    {
        constexpr char* const args[] = { (char*)"awk", (char*)"-F:", (char*)"{ print $1}", (char*)"/etc/passwd", nullptr };
        userRunner.init(args);
        userRunner.updateBufferSize();
        userRunner.update(true);
        //pid = UGM::Core::Utilities::loadLineByLineFromPID(usernamesBuffer, args, true, &tr);
        bFirst = false;
    }
    if (!bFirstTimeRunning)
    {
        userRunner.update();
        userRunner.updateBufferSize();
    }
    if (framesPassed > 30)
        framesPassed = 0;
    //if (tr.joinable())
    //{
    //    tr.join();
    //    kill(pid, SIGTERM);
    //}

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

    if (ImGui::Button("Exit"))
        mainWindow.closeWindow();
    ImGui::SameLine();
    if (ImGui::Button("Start") && bEnabled && bFirstTimeRunning)
    {
        bStartedScript = true;
        constexpr char* const args[] = { (char*)"lxc", (char*)"exec", (char*)"steam-ubuntu", (char*)"--", (char*)"bash", (char*)"-c", (char*)"su ubuntu -c 'ping -c 5 google.com'", nullptr };
        //constexpr char* const args[] = { (char*)"lxc", (char*)"exec", (char*)"steam-ubuntu", (char*)"--", (char*)"bash", (char*)"-c", (char*)"su ubuntu -c steam", nullptr };
        //scriptpid = UGM::Core::Utilities::loadLineByLineFromPID(messageBuffer, args, true, &scriptThread);
        installRunner.init(args);
        installRunner.updateBufferSize();
        installRunner.update(true);
        scriptRunOffloader = std::thread([&](){
            while (installRunner.valid() && !mainWindow.getWindowClose())
            {
                installRunner.updateBufferSize();
                installRunner.update();
                std::cout << "Test" << std::endl;
            }
        });
        bFirstTimeRunning = false;
    }
    // Since we're running at 60 FPS, we refresh once every 10 frames or 6 times a second
    if (bStartedScript)
    {
        ImGui::Separator();
        ImGui::BeginChild("##MessageLog");
        for (auto& a : installRunner.data())
            ImGui::Text("%s", a.c_str());
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
        ImGui::EndChild();
        if (!scriptRunOffloader.joinable())
            scriptRunOffloader.join();
        //if (scriptRunOffloader.joinable())
        //    scriptRunOffloader.join();
    }
}

void UGM::Installer::GUI::slide2(UGM::GUI::Window& mainWindow, uint8_t& currentSlide) noexcept
{

}

void UGM::Installer::GUI::slide3(UGM::GUI::Window& mainWindow, uint8_t& currentSlide) noexcept
{

}
