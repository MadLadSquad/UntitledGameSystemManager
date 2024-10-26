#pragma once
#include <Framework.hpp>

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API MainBar final : public UImGui::TitlebarComponent
    {
    public:
        MainBar() noexcept = default;
        virtual void begin() noexcept override;
        virtual void tick(float deltaTime) noexcept override;
        virtual void end() noexcept override;
        virtual ~MainBar() noexcept override = default;
    private:

    };
}

