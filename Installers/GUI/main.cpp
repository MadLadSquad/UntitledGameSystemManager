#include <Core/../../GUI/Window.hpp>
#include "GUI.hpp"
#include <Core/../../GUI/ImGuiUtils.hpp>

int main()
{
    UGM::GUI::Window window;
    window.init("Untitled Linux Game Manager GUI Installer", nullptr, UGM::Installer::GUI::render, [](UGM::GUI::Window&){});
    UGM::GUI::ImGuiUtils::init(window.data(), R"(
[Window][DockSpace Demo]
Pos=0,0
Size=800,600
Collapsed=0

[Window][Debug##Default]
Pos=60,60
Size=400,400
Collapsed=0

[Docking][Data]
DockSpace ID=0x3BC79352 Window=0x4647B76E Pos=2603,871 Size=800,4 CentralNode=1
)");
    while (!window.getWindowClose())
        window.tick();
    uint8_t tmp = 0;
    std::string tmpstr;
    UGM::Installer::GUI::slide1(window, tmp, true);
    UGM::Installer::GUI::slide2(tmpstr, window, tmp, true);
}