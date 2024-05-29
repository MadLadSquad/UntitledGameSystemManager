#pragma once
#include "Framework.hpp"
#include "Generated/Config.hpp"

namespace UntitledGameSystemManager 
{
    class UIMGUI_PUBLIC_API GenericErrorPopup  : public UImGui::WindowComponent 
    {
    public:
        GenericErrorPopup ();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~GenericErrorPopup () override;

        UImGui::FString popupName;
        UImGui::FString popupString;
    private:

    };
}
