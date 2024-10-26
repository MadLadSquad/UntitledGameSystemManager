#include "Instance.hpp"
#include <yaml-cpp/yaml.h>
#include "IncusBindings/libUGM_Incus_InternalFuncs.h"
#include "Interfaces/RendererInterface.hpp"

UntitledGameSystemManager::Instance::Instance() noexcept
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
            &del,
            &scriptGen,
            &connectionPopup,
            &genericErrorPopup
        },
        .globalData = static_cast<void*>(this),
        .bGlobalAllocatedOnHeap = false,
        UIMGUI_INIT_INFO_DEFAULT_DIRS,
    };

    auto configHome = UXDG::XDG_CONFIG_HOME();
    if (configHome.empty())
        configHome = UXDG::HOME() + "/.config";

    configDir += configHome + "/UntitledGameSystemManager/";
    if (!std::filesystem::exists(configDir))
        std::filesystem::create_directories(configDir);

    if (std::filesystem::exists(configDir + "scripts/"))
        std::filesystem::remove_all(configDir + "scripts/");
    std::filesystem::copy(UIMGUI_CONFIG_DIR"Dist", std::filesystem::path(configDir)/"scripts/");
}

void UntitledGameSystemManager::Instance::begin() noexcept
{
    beginAutohandle();
    loadConfigData();

    // Connect to Incus
    if (IncusCreateConnection() != 0)
    {
        connectionPopup.state = UIMGUI_COMPONENT_STATE_RUNNING;
        connectionPopup.error = IncusGetError();
    }
}

void UntitledGameSystemManager::Instance::tick(const float deltaTime) noexcept
{
    tickAutohandle(deltaTime);
    if (bWorkerActive && bFinishedExecution && worker.joinable())
    {
        worker.join();
        bWorkerActive = false;
        bFinishedExecution = false;
    }
}

void UntitledGameSystemManager::Instance::end() noexcept
{
    endAutohandle();
    if (worker.joinable())
        worker.join();
    if (connectionPopup.state == UIMGUI_COMPONENT_STATE_PAUSED)
        IncusDestroyConnection();
}

void UntitledGameSystemManager::Instance::onEventConfigureStyle(ImGuiStyle& style, ImGuiIO& io) noexcept
{
}

void UntitledGameSystemManager::Instance::loadConfigData()
{
    containers.clear();
    selectedContainer = nullptr;

    YAML::Node out = loadConfigGeneric();
    auto cont = out["containers"];
    if (cont)
    {
        for (YAML::Node a : cont)
        {
            if (a["container"] && a["pins"])
            {
                containers.emplace_back();
                for (auto& f : a["pins"].as<UImGui::TVector<UImGui::FString>>())
                    containers.back().pins.emplace_back( f, false );
                containers.back().name = a["container"].as<UImGui::FString>();
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
File in question: )", ULOG_LOG_TYPE_ERROR, configDir, "config/layout.yaml");
                std::terminate();
            }
        }
    }
    else
    {
        Logger::log("The config file isn't formatted correctly, it starts with a \"containers\" key that stores"
                    "an array of containers in YAML. File: ", ULOG_LOG_TYPE_ERROR, configDir, "config/layout.yaml");
        std::terminate();
    }
}

YAML::Node UntitledGameSystemManager::Instance::loadConfigGeneric()
{
    YAML::Node out;
    try
    {
        out = YAML::LoadFile((configDir + "config/layout.yaml").c_str());
    }
    catch (YAML::BadFile&)
    {
        Logger::log("Couldn't open the config file at: ", ULOG_LOG_TYPE_ERROR, configDir, "config/layout.yaml");
        std::terminate();
    }
    return out;
}

void UntitledGameSystemManager::Instance::outputConfig(const YAML::Node& node) const noexcept
{
    std::ofstream file((configDir + "config/layout.yaml").c_str());
    file << node;
}
