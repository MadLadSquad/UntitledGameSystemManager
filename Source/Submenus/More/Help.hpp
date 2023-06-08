#pragma once
#include <Framework.hpp>

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API Help : public UImGui::WindowComponent
    {
    public:
        Help();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~Help() override;
    private:

    };
}

