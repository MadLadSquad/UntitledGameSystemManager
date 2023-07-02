#pragma once
#include <Framework.hpp>

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API NewContainer : public UImGui::WindowComponent
    {
    public:
        NewContainer();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~NewContainer() override;
    private:
        bool bStartExecuting = false;
        std::string currentEvent;

        std::mutex mutex;
    };
}

