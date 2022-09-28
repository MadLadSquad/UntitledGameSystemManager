#pragma once
#include <Framework.hpp>

namespace UntitledLinuxGameManager
{
    class UIMGUI_PUBLIC_API ShowDirectories : public UImGui::WindowComponent
    {
    public:
        ShowDirectories();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~ShowDirectories() override;
    private:

    };
}

