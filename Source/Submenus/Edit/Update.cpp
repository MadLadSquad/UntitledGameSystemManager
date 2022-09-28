#include "Update.hpp"

UntitledLinuxGameManager::Update::Update()
{
    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledLinuxGameManager::Update::begin()
{
    beginAutohandle();

}

void UntitledLinuxGameManager::Update::tick(float deltaTime)
{
    tickAutohandle(deltaTime);

}

void UntitledLinuxGameManager::Update::end()
{
    endAutohandle();

}

UntitledLinuxGameManager::Update::~Update()
{

}

