#pragma once
#include <Framework.hpp>

namespace UntitledLinuxGameManager
{
    class UIMGUI_PUBLIC_API Poweroff : public UImGui::WindowComponent
    {
    public:
        Poweroff();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~Poweroff() override;
    private:

    };
}

