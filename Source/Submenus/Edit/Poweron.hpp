#pragma once
#include <Framework.hpp>

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API Poweron final : public UImGui::WindowComponent
    {
    public:
        Poweron() noexcept;
        virtual void begin() noexcept override;
        virtual void tick(float deltaTime) noexcept override;
        virtual void end() noexcept override;
        virtual ~Poweron() noexcept override = default;
    private:

    };
}

