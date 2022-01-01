#include <GL/glew.h>
#include "Window.hpp"
#include <glfw3.h>
#include <UVKLog.h>
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#include "ImGuiUtils.hpp"
#include <imgui.h>

UGM::GUI::Window::~Window()
{
    destroy();
}

void UGM::GUI::Window::init(const char* title, const char* imageName, const std::function<void(Window&)>& editorRenderFunc, const std::function<void(Window&)>& winRenderFunc)
{
    uiRenderFunc = editorRenderFunc;
    windowRenderFunc = winRenderFunc;
    // Crash the app when there is an error reported!
    logger.setCrashOnError(true);
    if (!glfwInit())
    {
        glfwTerminate();
        logger.consoleLog("GLFW initialization failed!", UVK_LOG_TYPE_ERROR);
    }
    logger.consoleLog("Setting up the window", UVK_LOG_TYPE_NOTE);

    // Init OpenGL
    glewExperimental = GL_TRUE;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 16);

    logger.consoleLog("Window settings configured", UVK_LOG_TYPE_NOTE);
    windowMain = glfwCreateWindow(800, 600, title, nullptr, nullptr);
    logger.consoleLog("Created window", UVK_LOG_TYPE_NOTE);

    if (imageName != nullptr)
    {
        GLFWimage images[1];
        images[0].pixels = stbi_load((std::string("~/.config/UntitledLinuxGameManager/") + imageName).c_str(), &images[0].width, &images[0].height, nullptr, 4);
        glfwSetWindowIcon(windowMain, 1, images);
        stbi_image_free(images[0].pixels);
    }

    if (!windowMain)
    {
        glfwTerminate();
        logger.consoleLog("GLFW window creation failed!", UVK_LOG_TYPE_ERROR);
        return;
    }
    logger.consoleLog("Window was created successfully", UVK_LOG_TYPE_SUCCESS);

    Core::Utilities::Vector2<int> tmp;
    glfwGetFramebufferSize(windowMain, &tmp.x, &tmp.y);

    glfwMakeContextCurrent(windowMain);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(windowMain, framebufferSizeCallback);

    if (glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(windowMain);
        glfwTerminate();
        logger.consoleLog("GLEW initialisation failed!", UVK_LOG_TYPE_ERROR);
        return;
    }
    glViewport(0, 0, tmp.x, tmp.y);
    glfwSetWindowUserPointer(windowMain, this);
}

void UGM::GUI::Window::destroy()
{
    ImGuiUtils::shutdown();
    glfwDestroyWindow(windowMain);
    glfwTerminate();
}

void UGM::GUI::Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    //auto* windowInst = static_cast<Window*>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);
}

bool UGM::GUI::Window::getWindowClose()
{
    return glfwWindowShouldClose(windowMain);
}

void UGM::GUI::Window::tick()
{
    glfwPollEvents();
    auto& colours = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
    glClearColor(colours.x, colours.y, colours.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGuiUtils::beginFrame();
    glUseProgram(0);
    ImGuiUtils::beginUI(uiRenderFunc, windowRenderFunc, *this);
    glfwSwapBuffers(windowMain);
}

GLFWwindow* UGM::GUI::Window::data()
{
    return windowMain;
}

void UGM::GUI::Window::closeWindow()
{
    glfwSetWindowShouldClose(windowMain, true);
}
