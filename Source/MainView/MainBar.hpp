#pragma once
#include <Framework.hpp>

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API MainBar : public UImGui::TitlebarComponent
    {
    public:
        MainBar();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~MainBar() override;
    private:

    };
}

