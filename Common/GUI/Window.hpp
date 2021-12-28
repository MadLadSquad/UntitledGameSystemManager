#pragma once
#include <Core.hpp>

struct GLFWwindow;
namespace UGM::GUI
{
    class Window
    {
    public:
        Window() = default;
        Window(const Window&) = delete;
        void operator=(Window const&) = delete;
        ~Window();

        void init();
    private:
        void destroy();

        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        GLFWwindow* windowMain = nullptr;
    };
}