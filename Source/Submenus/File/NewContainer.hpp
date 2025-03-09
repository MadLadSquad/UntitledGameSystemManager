#pragma once
#include <Framework.hpp>

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API NewContainer final : public UImGui::WindowComponent
    {
    public:
        NewContainer() noexcept;
        virtual void begin() noexcept override;
        virtual void tick(float deltaTime) override;
        virtual void end() noexcept override;
        virtual ~NewContainer() noexcept override = default;
    private:
        bool bStartExecuting = false;
        UImGui::FString currentEvent;
    };
}

