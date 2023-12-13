#pragma once

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <Abstractions/Rendering/BufferManager.h>
#include <Abstractions/PresentationManager.h>

class GuiManager
{
private:
    GuiManager(VkInstance instance, PresentationManager& presentMan, GLFWwindow* window,
                    VkQueue graphicsQueue, VkRenderPass renderPass, BufferManager& bufferMan)
    { setupImGui(instance, presentMan, window, graphicsQueue, renderPass, bufferMan); }

public:
    static GuiManager* guiInstance;
    static GuiManager* getInstance(VkInstance instance, PresentationManager presentMan, GLFWwindow* window,
                    VkQueue graphicsQueue, VkRenderPass renderPass, BufferManager& bufferMan)
    {
        if (!guiInstance) guiInstance = new GuiManager(instance, presentMan, window, graphicsQueue, renderPass, bufferMan);
        return guiInstance;
    }

    const ImGuiIO& io = ImGui::GetIO();
    VkImage fontImage{};

    void setupImGui(VkInstance instance, PresentationManager presentMan, GLFWwindow* window,
                    VkQueue graphicsQueue, VkRenderPass renderPass, BufferManager& bufferMan);
    void destroyImGui();

    static void newFrame();

private:
    BufferManager buffMan;
};
