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

        void init(const char* title, const char* imageName, const std::function<void(Window&)>& editorRenderFunc);
        bool getWindowClose();
        void closeWindow();
        void tick();
        GLFWwindow* data();
    private:
        void destroy();

        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        GLFWwindow* windowMain = nullptr;
        std::function<void(Window&)> uiRenderFunc;
    };
}