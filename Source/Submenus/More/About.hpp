#pragma once
#include <Framework.hpp>

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API About : public UImGui::WindowComponent
    {
    public:
        About();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~About() override;
    private:
        UImGui::Texture brandIcon;
        UImGui::Texture lxcIcon;
    };
}

