#include "Instance.hpp"
#include <unistd.h>
#include <pwd.h>
#include <yaml-cpp/yaml.h>
#include "LXDBindings/libUGM_LXD_InternalFuncs.h"
#include "Interfaces/RendererInterface.hpp"

UntitledLinuxGameManager::Instance::Instance()
{
    initInfo = UImGui::InitInfo
    {
        .titlebarComponents =
        {
            &mainBar
        },
        .windowComponents =
        {
            &mainView,
            &sidePanel,
            &pin,
            &poweroff,
            &poweron,
            &restart,
            &update,
            &exitWidget,
            &newContainer,
            &help,
            &about,
            &del
        },
        .globalData = (void*)this,
        .bGlobalAllocatedOnHeap = false,
    };

    // Check for XDG_CONFIG_HOME
    const char* configHome = std::getenv("XDG_CONFIG_HOME");
    if (configHome == nullptr)
    {
        // No XDG_CONFIG_HOME? No problem! Simply use HOME
        configHome = std::getenv("HOME");
        if (configHome == nullptr) // NO HOME!?!??!?!??!?!?! Use what we can ig
            configDir = std::string("/home/") + getpwuid(geteuid())->pw_name;
        else
            configDir = std::string(configHome);
        configDir += "/.config/";
    }
    else
        configDir += std::string(configHome) + "/";
    configDir += "UntitledLinuxGameManager/";
}

void UntitledLinuxGameManager::Instance::begin()
{
    beginAutohandle();
    loadConfigData();

    // Connect to LXD
    if (LXDCreateConnection() != 0)
    {
        Logger::log("Failed to establish connection to LXD. Error: ", UVKLog::UVK_LOG_TYPE_ERROR, LXDGetError());
        UImGui::Instance::shutdown();
    }

    gpuType = UImGui::Renderer::getGPUName()[0] == 'N' ? 'N' : 'M';
}

void UntitledLinuxGameManager::Instance::tick(float deltaTime)
{
    tickAutohandle(deltaTime);
    if (bWorkerActive && bFinishedExecution && worker.joinable())
    {
        worker.join();
        bWorkerActive = false;
        bFinishedExecution = false;
    }
}

void UntitledLinuxGameManager::Instance::end()
{
    endAutohandle();
    if (worker.joinable())
        worker.join();
    LXDDestroyConnection();
}

UntitledLinuxGameManager::Instance::~Instance()
{

}

void UntitledLinuxGameManager::Instance::onEventConfigureStyle(ImGuiStyle& style, ImGuiIO& io)
{
}

void UntitledLinuxGameManager::Instance::loadConfigData()
{
    containers.clear();

    YAML::Node out;
    try
    {
        out = YAML::LoadFile(configDir + "config/layout.yaml");
    }
    catch (YAML::BadFile&)
    {
        Logger::log("Couldn't open the config file at: ", UVKLog::UVK_LOG_TYPE_ERROR, configDir, "config/layout.yaml");
        std::terminate();
    }
    auto cont = out["containers"];
    if (cont)
    {
        for (YAML::Node a : cont)
        {
            if (a["container"] && a["pins"])
            {
                containers.push_back({});
                for (auto& f : a["pins"].as<std::vector<std::string>>())
                    containers.back().pins.emplace_back( f, false );
                containers.back().name = a["container"].as<std::string>();
            }
            else
            {
                Logger::log(R"(The config file isn't formatted correctly! The file should look like this, with
every member of the "containers" array having a "container" and "pins" key:

<begin file>
containers:
  - container: steam-arch
    pins:
      - steam
      - lutris
      - firefox
      - arma3-unix-launcher
      - discord
<endfile>
File in question: )", UVKLog::UVK_LOG_TYPE_ERROR, configDir, "config/layout.yaml");
                std::terminate();
            }
        }
    }
    else
    {
        Logger::log("The config file isn't formatted correctly, it starts with a \"containers\" key that stores"
                    "an array of containers in YAML. File: ", UVKLog::UVK_LOG_TYPE_ERROR, configDir, "config/layout.yaml");
        std::terminate();
    }
}