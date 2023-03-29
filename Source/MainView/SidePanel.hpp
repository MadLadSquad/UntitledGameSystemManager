#pragma once
#include <Framework.hpp>

namespace UntitledLinuxGameManager
{
    class UIMGUI_PUBLIC_API SidePanel : public UImGui::WindowComponent
    {
    public:
        SidePanel();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~SidePanel() override;
    private:
    };
}

