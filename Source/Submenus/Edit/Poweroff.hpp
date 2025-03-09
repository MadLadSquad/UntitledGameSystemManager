#pragma once
#include <Framework.hpp>

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API Poweroff final : public UImGui::WindowComponent
    {
    public:
        Poweroff() noexcept;
        virtual void begin() noexcept override;
        virtual void tick(float deltaTime) noexcept override;
        virtual void end() noexcept override;
        virtual ~Poweroff() noexcept override = default;
    private:
    };
}

