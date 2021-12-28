#include "Window.hpp"
#include <glfw3.h>

UGM::GUI::Window::~Window()
{
    destroy();
}

void UGM::GUI::Window::init()
{
    if (!glfwInit())
    {
        logger.consoleLog("GLFW initialization failed!", UVK_LOG_TYPE_ERROR);
        glfwTerminate();
        return;
    }

}

void UGM::GUI::Window::destroy()
{

}

void UGM::GUI::Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    //auto* windowInst = static_cast<Window*>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);
}
