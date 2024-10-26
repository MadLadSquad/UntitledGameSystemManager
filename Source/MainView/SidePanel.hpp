#pragma once
#include <Framework.hpp>

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API SidePanel final : public UImGui::WindowComponent
    {
    public:
        SidePanel() noexcept = default;
        virtual void begin() noexcept override;
        virtual void tick(float deltaTime) noexcept override;
        virtual void end() noexcept override;
        virtual ~SidePanel() noexcept override = default;
    private:
    };
}

