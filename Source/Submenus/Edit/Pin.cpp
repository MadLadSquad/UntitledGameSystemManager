#include "Pin.hpp"

UntitledGameSystemManager::Pin::Pin()
{
    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::Pin::begin()
{
    beginAutohandle();

}

void UntitledGameSystemManager::Pin::tick(float deltaTime)
{
    tickAutohandle(deltaTime);

}

void UntitledGameSystemManager::Pin::end()
{
    endAutohandle();

}

UntitledGameSystemManager::Pin::~Pin()
{

}

