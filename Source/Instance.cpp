#include "Instance.hpp"

UntitledLinuxGameManager::Instance::Instance()
{
    initInfo = UImGui::InitInfo
    {
        .titlebarComponents =
        {
            &mainBar
        },
        .windowComponents =
        {
            &mainView,
            &sidePanel,
            &pin,
            &poweroff,
            &poweron,
            &restart,
            &update,
            &exitWidget,
            &newContainer,
            &help,
            &about,
        },
        .globalData = (void*)this,
    };
}

void UntitledLinuxGameManager::Instance::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::Instance::tick(float deltaTime)
{
    tickAutohandle(deltaTime);

}

void UntitledLinuxGameManager::Instance::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::Instance::~Instance()
{

}

void UntitledLinuxGameManager::Instance::onEventConfigureStyle(ImGuiStyle& style, ImGuiIO& io)
{
}