#pragma once
#include <Framework.hpp>

namespace UntitledGameSystemManager
{
    class UIMGUI_PUBLIC_API Update : public UImGui::WindowComponent
    {
    public:
        Update();
        virtual void begin() override;
        virtual void tick(float deltaTime) override;
        virtual void end() override;
        virtual ~Update() override;
    private:
        bool bStartExecuting = false;
        UImGui::FString currentEvent;
        std::mutex mutex;
    };
}

