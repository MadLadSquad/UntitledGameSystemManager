#pragma once
#include <Framework.hpp>
#include <Generated/Config.hpp>

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API About final : public UImGui::WindowComponent
    {
    public:
        About() noexcept;
        virtual void begin() noexcept override;
        virtual void tick(float deltaTime) noexcept override;
        virtual void end() noexcept override;
        virtual ~About() noexcept override = default;
    private:
        UImGui::Texture brandIcon;
        UImGui::Texture lxcIcon;
    };
}

