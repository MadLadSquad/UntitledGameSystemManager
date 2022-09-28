#include "Restart.hpp"

UntitledLinuxGameManager::Restart::Restart()
{
    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledLinuxGameManager::Restart::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::Restart::tick(float deltaTime)
{
    tickAutohandle(deltaTime);

}

void UntitledLinuxGameManager::Restart::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::Restart::~Restart()
{

}

