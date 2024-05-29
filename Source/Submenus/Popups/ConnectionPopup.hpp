#pragma once
#include "Framework.hpp"
#include "Generated/Config.hpp"

namespace UntitledGameSystemManager 
{
    class UIMGUI_PUBLIC_API ConnectionPopup  : public UImGui::WindowComponent 
    {
    public:
        ConnectionPopup ();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~ConnectionPopup () override;
    private:
        friend class Instance;

        UImGui::FString error;
    };
}
