#include "Poweroff.hpp"

UntitledLinuxGameManager::Poweroff::Poweroff()
{
    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledLinuxGameManager::Poweroff::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::Poweroff::tick(float deltaTime)
{
    tickAutohandle(deltaTime);

}

void UntitledLinuxGameManager::Poweroff::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::Poweroff::~Poweroff()
{

}

