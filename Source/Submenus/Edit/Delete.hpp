#pragma once
#include "Framework.hpp"

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API Delete : public UImGui::WindowComponent
    {
    public:
        Delete();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~Delete() override;
    private:

    };
}

