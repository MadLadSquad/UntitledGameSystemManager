#include "Poweron.hpp"

UntitledLinuxGameManager::Poweron::Poweron()
{
    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledLinuxGameManager::Poweron::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::Poweron::tick(float deltaTime)
{
    tickAutohandle(deltaTime);

}

void UntitledLinuxGameManager::Poweron::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::Poweron::~Poweron()
{

}

