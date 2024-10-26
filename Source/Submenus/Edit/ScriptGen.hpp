#pragma once
#include "Framework.hpp"
#include "Generated/Config.hpp"

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API ScriptGen final : public UImGui::WindowComponent
    {
    public:
        ScriptGen() noexcept;
        virtual void begin() noexcept override;
        virtual void tick(float deltaTime) noexcept override;
        virtual void end() noexcept override;
        virtual ~ScriptGen() noexcept override = default;
    private:
        static void generateScript(const UImGui::FString& command) noexcept;

    };
}

