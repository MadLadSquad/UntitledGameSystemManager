#include "Update.hpp"

UntitledGameSystemManager::Update::Update()
{
    state = UImGui::UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::Update::begin()
{
    beginAutohandle();

}

void UntitledGameSystemManager::Update::tick(float deltaTime)
{
    tickAutohandle(deltaTime);

}

void UntitledGameSystemManager::Update::end()
{
    endAutohandle();

}

UntitledGameSystemManager::Update::~Update()
{

}

