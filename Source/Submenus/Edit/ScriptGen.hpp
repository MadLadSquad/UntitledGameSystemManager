#pragma once
#include "Framework.hpp"
#include "Generated/Config.hpp"

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API ScriptGen : public UImGui::WindowComponent
    {
    public:
        ScriptGen();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~ScriptGen() override;
    private:
        static void generateScript(const std::string& command) noexcept;

    };
}

