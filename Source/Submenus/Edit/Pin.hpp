#pragma once
#include <Framework.hpp>

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API Pin : public UImGui::WindowComponent
    {
    public:
        Pin();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~Pin() override;

        static void generateScript(const std::string& str, const std::string& command) noexcept;
    private:

    };
}

