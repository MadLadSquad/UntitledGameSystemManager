#include "Exit.hpp"

UntitledLinuxGameManager::Exit::Exit()
{
    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledLinuxGameManager::Exit::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::Exit::tick(float deltaTime)
{
    tickAutohandle(deltaTime);

}

void UntitledLinuxGameManager::Exit::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::Exit::~Exit()
{

}

