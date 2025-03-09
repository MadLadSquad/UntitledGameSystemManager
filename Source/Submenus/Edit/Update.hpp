#pragma once
#include <Framework.hpp>

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API Update final : public UImGui::WindowComponent
    {
    public:
        Update() noexcept;
        virtual void begin() noexcept override;
        virtual void tick(float deltaTime) noexcept override;
        virtual void end() noexcept override;
        virtual ~Update() noexcept override = default;
    private:
        bool bStartExecuting = false;
        UImGui::FString currentEvent;
    };
}

