#pragma once
#include <Framework.hpp>
#include "MainView/Widgets.hpp"
#include "Submenus/Widgets.hpp"

namespace UntitledLinuxGameManager
{
    class UIMGUI_PUBLIC_API Instance : public UImGui::Instance
    {
    public:
        Instance();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~Instance() override;

        virtual void onEventConfigureStyle(ImGuiStyle& style, ImGuiIO& io) override;
    private:
        friend class MainBar;

        Pin pin{};
        Poweroff poweroff{};
        Poweron poweron{};
        Restart restart{};
        Update update{};
        Exit exitWidget{};
        NewContainer newContainer{};
        Help help{};
        About about{};

        MainBar mainBar{};
        MainView mainView{};
        SidePanel sidePanel{};
    };
}

