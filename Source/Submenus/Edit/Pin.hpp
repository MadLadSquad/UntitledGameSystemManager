#pragma once
#include <Framework.hpp>

namespace UntitledLinuxGameManager
{
    class UIMGUI_PUBLIC_API Pin : public UImGui::WindowComponent
    {
    public:
        Pin();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~Pin() override;
    private:

    };
}
