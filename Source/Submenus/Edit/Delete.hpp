#pragma once
#include "Framework.hpp"

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API Delete final : public UImGui::WindowComponent
    {
    public:
        Delete() noexcept;
        virtual void begin() noexcept override;
        virtual void tick(float deltaTime) noexcept override;
        virtual void end() noexcept override;
        virtual ~Delete() noexcept override = default;
    private:
        std::mutex mutex;
    };
}

