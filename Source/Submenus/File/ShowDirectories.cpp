#include "ShowDirectories.hpp"

UntitledLinuxGameManager::ShowDirectories::ShowDirectories()
{
    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledLinuxGameManager::ShowDirectories::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::ShowDirectories::tick(float deltaTime)
{
    tickAutohandle(deltaTime);

}

void UntitledLinuxGameManager::ShowDirectories::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::ShowDirectories::~ShowDirectories()
{

}

