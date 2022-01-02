#include <Core/../../GUI/Window.hpp>
#include "GUI.hpp"
#include <Core/../../GUI/ImGuiUtils.hpp>

int main()
{
    UGM::GUI::Window window;
    window.init("Untitled Linux Game Manager GUI Installer", nullptr, UGM::Installer::GUI::render, [](UGM::GUI::Window&){});
    UGM::GUI::ImGuiUtils::init(window.data());
    while (!window.getWindowClose())
        window.tick();
    uint8_t tmp = 0;
    std::string tmpstr;
    UGM::Installer::GUI::slide1(window, tmp, true);
    UGM::Installer::GUI::slide2(tmpstr, window, tmp, true);
}