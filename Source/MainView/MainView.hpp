#pragma once
#include <Framework.hpp>

namespace UntitledLinuxGameManager
{
    class UIMGUI_PUBLIC_API MainView : public UImGui::WindowComponent
    {
    public:
        MainView();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~MainView() override;
    private:
        // Here we want pins to just be the name of the pin and its selected state, true if selected, false if not
        using Pin = std::pair<std::string, bool>;

        // Contain pins in memory here
        std::vector<Pin> pins = { { "test", true }, { "test2", false } };
    };
}

