#pragma once
#include <Framework.hpp>
#include <Generated/Config.hpp>
#include "MainView/Widgets.hpp"
#include "Submenus/Widgets.hpp"
#include <thread>
#include "IncusBindings/libUGM_Incus_InternalFuncs.h"

#define INCUS_RUN(x, y, z, ...)                                                                                 \
    if (x(y __VA_OPT__(,) __VA_ARGS__) != 0)                                                                    \
    {                                                                                                           \
        inst->genericErrorPopup.state = UIMGUI_COMPONENT_STATE_RUNNING;                                         \
        inst->genericErrorPopup.popupName = UImGui::FString("Failed to " z " the following container: ") + y;   \
        inst->genericErrorPopup.popupString = inst->genericErrorPopup.popupName + " Error: " + IncusGetError(); \
    }


#define INCUS_RUN_AND_CLOSE(x, y) inst->bWorkerActive = true;                                           \
    inst->worker = std::thread([&]() -> void { INCUS_RUN(x, inst->selectedContainer->name.data(), y);   \
    state = UIMGUI_COMPONENT_STATE_PAUSED;                                                              \
    ((Instance*)UImGui::Instance::getGlobal())->bFinishedExecution = true;                              \
});

namespace UntitledGameSystemManager
{
    struct Container
    {
        using Pin = std::pair<UImGui::FString, bool>;

        UImGui::FString name;
        UImGui::TVector<Pin> pins;
    };

    struct RunWorkers
    {
        std::thread thread;
        UImGui::FString name;
        bool bWorkerActive = false;
        bool bFinishedExecution = false;
    };

    class UIMGUI_PUBLIC_API Instance final : public UImGui::Instance
    {
    public:
        Instance() noexcept;
        virtual void begin() noexcept override;
        virtual void tick(float deltaTime) noexcept override;
        virtual void end() noexcept override;
        virtual ~Instance() noexcept override = default;

        virtual void onEventConfigureStyle(ImGuiStyle& style, ImGuiIO& io) noexcept override;

        void loadConfigData();

        UImGui::TVector<Container> containers;
        Container* selectedContainer = nullptr;

        UImGui::FString configDir;

        std::thread worker;
        bool bWorkerActive = false;
        bool bFinishedExecution = false;

        GenericErrorPopup genericErrorPopup{};

        YAML::Node loadConfigGeneric();
        void outputConfig(const YAML::Node& node) const noexcept;
    private:
        friend class MainBar;
        friend class MainView;
        friend class SidePanel;

        Pin pin{};
        Poweroff poweroff{};
        Poweron poweron{};
        Restart restart{};
        ScriptGen scriptGen{};
        Update update{};
        Exit exitWidget{};
        NewContainer newContainer{};
        Help help{};
        About about{};
        Delete del{};

        MainBar mainBar{};
        MainView mainView{};
        SidePanel sidePanel{};

        ConnectionPopup connectionPopup{};
    };
}

