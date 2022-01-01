#pragma once
#include <Core/../../GUI/Window.hpp>

namespace UGM::Managers::GUI
{
    void render(UGM::GUI::Window& mainWindow);

    void renderAboutUs(UGM::GUI::Window& mainWindow, bool& bOpen);
    void renderInfo(UGM::GUI::Window& mainWindow, bool& bOpen);
    void renderNew(UGM::GUI::Window& mainWindow, bool& bOpen);
    void renderRestart(UGM::GUI::Window& mainWindow, bool& bOpen);
    void renderPoweroff(UGM::GUI::Window& mainWindow, bool& bOpen);
    void renderPoweron(UGM::GUI::Window& mainWindow, bool& bOpen);
    void renderExit(UGM::GUI::Window& mainWindow, bool& bOpen);
    void renderDirectories(UGM::GUI::Window& mainWindow, bool& bOpen);

    std::string* renderSidebar(UGM::GUI::Window& mainWindow);
    void renderMainView(UGM::GUI::Window& mainWindow, std::string* seletedContainer, const bool& bReset = false);

    void renderWindows(UGM::GUI::Window& mainWindow);
}