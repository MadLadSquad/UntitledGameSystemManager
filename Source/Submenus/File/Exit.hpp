#pragma once
#include <Framework.hpp>

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API Exit : public UImGui::WindowComponent
    {
    public:
        Exit();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~Exit() override;
    private:

    };
}

