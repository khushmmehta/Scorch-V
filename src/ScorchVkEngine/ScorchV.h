#pragma once
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#include <vector>

#include <GLFW/glfw3.h>

#include <Abstractions/ValidationLayers.h>
#include <Abstractions/PresentationManager.h>
#include <Abstractions/Rendering/BufferManager.h>
#include <Abstractions/Rendering/Objects/MeshObject.h>
#include <Abstractions/GuiManager.h>

class ScorchV
{
public:
    void run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

    bool frameBufferResized = false;

private:
    #pragma region Objects
    GLFWwindow* window{};

    VkInstance instance{};
    ValidationLayers vLayers;

    PresentationManager presentMan{};
    QueueFamilyIndices _indices;

    MeshObject mesh;

    VkQueue graphicsQueue{};
    VkQueue presentQueue{};

    VkRenderPass renderPass{};

    VkPipelineLayout pipelineLayout{};
    VkPipeline graphicsPipeline{};

    std::vector<VkCommandPool> commandPools{};

    BufferManager bufferMan;

    GuiManager* guiMan = GuiManager::getInstance();

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

    #pragma endregion

    void initWindow();

    void initVulkan()
    {
        createInstance();
        vLayers.setupDebugMessenger(instance);
        presentMan.setUpPresentation(instance, window, vLayers, graphicsQueue, presentQueue);
        createRenderPass();
        bufferMan.createDescriptorSetLayout(presentMan.device);
        createGraphicsPipeline();
        presentMan.createFramebuffers(renderPass);
        createCommandPools();
        // Here is where I provide my meshdata to create their buffers
        bufferMan.setUpBufferManager(presentMan.physicalDevice, presentMan.device, instance, mesh.vertices, mesh.indices, commandPools[0], graphicsQueue);
        createCommandBuffers();
        createSyncObjects();
        guiMan->setupImGui(instance, presentMan, window, graphicsQueue, renderPass, bufferMan);
    }

    void mainLoop();
    void cleanup();

    void createInstance();
    void createRenderPass();
    void createGraphicsPipeline();
    void createCommandPools();

    void createVkCommandPool(VkCommandPool& commandPool, VkCommandPoolCreateFlags flags)
    {
        const QueueFamilyIndices queueFamilyIndices = presentMan.findQueueFamilies(presentMan.physicalDevice);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = flags;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(presentMan.device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
            throw std::runtime_error("Failed to create the command pool!");
    }

    void createCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void createSyncObjects();
    void drawFrame();
};
