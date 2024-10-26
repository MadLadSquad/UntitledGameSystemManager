#pragma once
#include <Framework.hpp>

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API MainView final : public UImGui::WindowComponent
    {
    public:
        MainView() noexcept = default;
        virtual void begin() noexcept override;
        virtual void tick(float deltaTime) noexcept override;
        virtual void end() noexcept override;
        virtual ~MainView() noexcept override = default;
    private:

    };
}

