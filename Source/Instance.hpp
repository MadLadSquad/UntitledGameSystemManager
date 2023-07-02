#pragma once
#include <Framework.hpp>
#include <Generated/Config.hpp>
#include "MainView/Widgets.hpp"
#include "Submenus/Widgets.hpp"
#include <thread>
#include "LXDBindings/libUGM_LXD_InternalFuncs.h"

namespace UntitledGameSystemManager
{
    struct Container
    {
        using Pin = std::pair<std::string, bool>;

        std::string name;
        std::vector<Pin> pins;
    };

    struct RunWorkers
    {
        std::thread thread;
        UImGui::FString name;
        bool bWorkerActive = false;
        bool bFinishedExecution = false;
    };

    class UIMGUI_PUBLIC_API Instance : public UImGui::Instance
    {
    public:
        Instance();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~Instance() override;

        virtual void onEventConfigureStyle(ImGuiStyle& style, ImGuiIO& io) override;

        void loadConfigData();

        std::vector<Container> containers;
        Container* selectedContainer = nullptr;

        UImGui::FString configDir;

        std::thread worker;
        bool bWorkerActive = false;
        bool bFinishedExecution = false;

        char gpuType = 'M';
    private:
        friend class MainBar;
        friend class MainView;
        friend class SidePanel;

        Pin pin{};
        Poweroff poweroff{};
        Poweron poweron{};
        Restart restart{};
        Update update{};
        Exit exitWidget{};
        NewContainer newContainer{};
        Help help{};
        About about{};
        Delete del{};

        MainBar mainBar{};
        MainView mainView{};
        SidePanel sidePanel{};
    };
}

