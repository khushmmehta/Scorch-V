#pragma once

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <Abstractions/Rendering/BufferManager.h>
#include <Abstractions/PresentationManager.h>

class GuiManager
{
private:
    GuiManager() {}

public:
    static GuiManager* guiInstance;
    static GuiManager* getInstance()
    {
        if (!guiInstance) guiInstance = new GuiManager();
        return guiInstance;
    }

    VkImage fontImage{};

    void setupImGui(VkInstance instance, PresentationManager presentMan, GLFWwindow* window,
                    VkQueue graphicsQueue, VkRenderPass renderPass, BufferManager& bufferMan);
    void destroyImGui(VkDevice device);

    static void newFrame();

private:
    BufferManager buffMan;
    VkDescriptorPool imguiPool{};
};
