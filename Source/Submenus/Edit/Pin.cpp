#include "Pin.hpp"

UntitledLinuxGameManager::Pin::Pin()
{
    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledLinuxGameManager::Pin::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::Pin::tick(float deltaTime)
{
    tickAutohandle(deltaTime);

}

void UntitledLinuxGameManager::Pin::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::Pin::~Pin()
{

}

