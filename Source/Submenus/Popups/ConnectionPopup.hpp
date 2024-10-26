#pragma once
#include "Framework.hpp"
#include "Generated/Config.hpp"

namespace UntitledGameSystemManager 
{
    class UIMGUI_PUBLIC_API ConnectionPopup final : public UImGui::WindowComponent
    {
    public:
        ConnectionPopup() noexcept;
        virtual void begin() noexcept override;
        virtual void tick(float deltaTime) noexcept override;
        virtual void end() noexcept override;
        virtual ~ConnectionPopup() noexcept override = default;
    private:
        friend class Instance;

        UImGui::FString error;
    };
}
