#pragma once
#include <Framework.hpp>

namespace UntitledLinuxGameManager
{
    class UIMGUI_PUBLIC_API Poweron : public UImGui::WindowComponent
    {
    public:
        Poweron();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~Poweron() override;
    private:

    };
}

