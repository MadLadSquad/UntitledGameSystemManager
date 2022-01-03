#pragma once
#include <Core/../../GUI/Window.hpp>

namespace UGM::Managers::GUI
{
    void render(UGM::GUI::Window& mainWindow);

    void renderAboutUs(UGM::GUI::Window& mainWindow, bool& bOpen);
    void renderNew(UGM::GUI::Window& mainWindow, bool& bOpen);
    void renderRestart(UGM::GUI::Window& mainWindow, bool& bOpen);
    void renderPoweroff(UGM::GUI::Window& mainWindow, bool& bOpen);
    void renderPoweron(UGM::GUI::Window& mainWindow, bool& bOpen);
    void renderExit(UGM::GUI::Window& mainWindow, bool& bOpen);
    void renderDirectories(UGM::GUI::Window& mainWindow, bool& bOpen);
    void renderDelete(UGM::GUI::Window& mainWindow, bool& bOpen);
    void renderExec(UGM::GUI::Window& mainWindow, bool& bOpen);

    void renderPin(UGM::GUI::Window& mainWindow, bool& bOpen);

    std::string* renderSidebar(UGM::GUI::Window& mainWindow);
    void renderMainView(UGM::GUI::Window& mainWindow, std::string* selectedContainer, const bool& bReset = false);

    void renderWindows(UGM::GUI::Window& mainWindow);

    void refreshPins(std::vector<std::pair<std::string, bool>>& pins);

    // Ugly globals here
    inline bool bShowNew = false;
    inline bool bShowPin = false;
    inline bool bShowDelete = false;
    inline bool bShowPoweron = false;
    inline bool bShowPoweroff = false;
    inline bool bShowRestart = false;

    inline std::string* selectedContainerG = nullptr;
}