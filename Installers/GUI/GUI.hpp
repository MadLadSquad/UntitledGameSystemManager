#pragma once
#include <iostream>

namespace UGM::GUI
{
    class Window;
}

namespace UGM::Installer::GUI
{
    void render(UGM::GUI::Window& mainWindow);

    void slide0(UGM::GUI::Window& mainWindow, uint8_t& currentSlide) noexcept;
    const std::string& slide1(UGM::GUI::Window& mainWindow, uint8_t& currentSlide, bool cleanup = false) noexcept;
    void slide2(std::string& user, UGM::GUI::Window& mainWindow, uint8_t& currentSlide, bool cleanup = false) noexcept;
}