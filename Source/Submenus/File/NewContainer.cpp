#include "NewContainer.hpp"
#include "Instance.hpp"

UntitledGameSystemManager::NewContainer::NewContainer()
{
    state = UIMGUI_COMPONENT_STATE_PAUSED;
}

void UntitledGameSystemManager::NewContainer::begin()
{
    beginAutohandle();

}

void UntitledGameSystemManager::NewContainer::tick(float deltaTime)
{
    tickAutohandle(deltaTime);
    auto* inst = (Instance*)UImGui::Instance::getGlobal();

    if (!ImGui::IsPopupOpen("New container"))
        ImGui::OpenPopup("New container");
    if (ImGui::BeginPopupModal("New container", (bool*)nullptr))
    {
        static std::string name;
        static std::string gpupreview;
        static char gpuTypeArgument;

        if (bStartExecuting)
        {
            ImGui::TextWrapped("Started creating a new container");
            ImGui::TextWrapped("Current step: %s", currentEvent.c_str());
        }
        else
        {
            ImGui::Text("Container name: ");
            ImGui::SameLine();
            ImGui::InputText("##containernameinputbox", &name);

            ImGui::Text("GPU Vendor: ");
            ImGui::SameLine();
            if (ImGui::BeginCombo("##selectgpu", gpupreview.c_str()))
            {
                if (ImGui::MenuItem("NVidia"))
                {
                    gpupreview = "NVidia";
                    gpuTypeArgument = 'N';
                }
                else if (ImGui::MenuItem("AMD"))
                {
                    gpupreview = "AMD";
                    gpuTypeArgument = 'M';
                }
                ImGui::EndCombo();
            }
        }


        if (!inst->bWorkerActive && !bStartExecuting && !name.empty())
        {
            if (ImGui::Button("New container##button"))
            {
                inst->bWorkerActive = true;
                inst->worker = std::thread([&]() -> void
                {
                    bStartExecuting = true;
                    currentEvent = "Creating a new container!";
                    if (LXDNewContainer(name.data(), (char*)"archlinux") != 0)
                    {
                        Logger::log("Failed to create the following container: ", UVKLog::UVK_LOG_TYPE_ERROR,
                                    name, " Error: ", LXDGetError());
                        UImGui::Instance::shutdown();
                    }

                    state = UIMGUI_COMPONENT_STATE_PAUSED;
                    ((Instance*)UImGui::Instance::getGlobal())->bFinishedExecution = true;
                });
            }
            else
                ImGui::SameLine();
        }
        if (!inst->bWorkerActive && !bStartExecuting && ImGui::Button("Close##info"))
            state = UIMGUI_COMPONENT_STATE_PAUSED;
        ImGui::EndPopup();
    }
}

void UntitledGameSystemManager::NewContainer::end()
{
    endAutohandle();
}

UntitledGameSystemManager::NewContainer::~NewContainer()
{

}

