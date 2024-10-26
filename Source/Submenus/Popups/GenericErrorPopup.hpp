#pragma once
#include "Framework.hpp"
#include "Generated/Config.hpp"

namespace UntitledGameSystemManager 
{
    class UIMGUI_PUBLIC_API GenericErrorPopup final : public UImGui::WindowComponent
    {
    public:
        GenericErrorPopup() noexcept;
        virtual void begin() noexcept override;
        virtual void tick(float deltaTime) noexcept override;
        virtual void end() noexcept override;
        virtual ~GenericErrorPopup() noexcept override = default;

        UImGui::FString popupName;
        UImGui::FString popupString;
    private:

    };
}
