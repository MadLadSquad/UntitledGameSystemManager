#pragma once
#include <Framework.hpp>

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API MainView : public UImGui::WindowComponent
    {
    public:
        MainView();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~MainView() override;
    private:

    };
}

