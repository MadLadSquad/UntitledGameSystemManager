#include <Core/../../GUI/Window.hpp>
#include "GUI.hpp"
#include <Core/../../GUI/ImGuiUtils.hpp>

int main()
{
    UGM::GUI::Window window;
    window.init("Untitled Linux Game Manager GUI Installer", nullptr, UGM::Installer::GUI::render);
    UGM::GUI::ImGuiUtils::init(window.data());
    while (!window.getWindowClose())
        window.tick();
}