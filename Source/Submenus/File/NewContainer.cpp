#include "NewContainer.hpp"

UntitledLinuxGameManager::NewContainer::NewContainer()
{
    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledLinuxGameManager::NewContainer::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::NewContainer::tick(float deltaTime)
{
    tickAutohandle(deltaTime);

}

void UntitledLinuxGameManager::NewContainer::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::NewContainer::~NewContainer()
{

}

