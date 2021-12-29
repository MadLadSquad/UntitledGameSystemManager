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
    void slide1(UGM::GUI::Window& mainWindow, uint8_t& currentSlide) noexcept;
    void slide2(UGM::GUI::Window& mainWindow, uint8_t& currentSlide) noexcept;
    void slide3(UGM::GUI::Window& mainWindow, uint8_t& currentSlide) noexcept;
}