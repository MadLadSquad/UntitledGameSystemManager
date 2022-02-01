#pragma once
#include <functional>
#include <iostream>

struct ImVec4;
struct GLFWwindow;

namespace UGM::GUI
{
    class Window;
    class ImGuiUtils
    {
    public:
        ImGuiUtils() = default;

        static void init(GLFWwindow* glfwwindow, const std::string& ini);
        static void beginUI(const std::function<void(Window&)>& renderFunc, const std::function<void(Window&)>& renderWindowFunc, Window& mainWindow);
        static void setupTheme(ImVec4* colours);
        static void beginFrame();
        static void shutdown();
    private:
    };
}