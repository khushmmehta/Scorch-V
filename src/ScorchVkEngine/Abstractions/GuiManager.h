// ReSharper disable CppUnusedIncludeDirective
#pragma once

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <Abstractions/Rendering/BufferManager.h>
#include <Abstractions/PresentationManager.h>

class GuiManager
{
public:
    static GuiManager* instance;
    static GuiManager* getInstance()
    {
        if (!instance) instance = new GuiManager();
        return instance;
    }

    VkImage fontImage{};

    void setupImGui(VkInstance instance, GLFWwindow* window, VkQueue graphicsQueue, VkRenderPass renderPass);
    void destroyImGui();

    static void newFrame();
    static void renderGui();

private:
    GuiManager() {}

    PresentationManager* presentMan = PresentationManager::getInstance();
    BufferManager* bufferMan = BufferManager::getInstance();
    VkDescriptorPool imguiPool{};
};
