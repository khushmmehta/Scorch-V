#pragma once
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#include <vector>

#include <GLFW/glfw3.h>

#include <Abstractions/ValidationLayers.h>
#include <Abstractions/PresentationManager.h>
#include <Abstractions/Rendering/BufferManager.h>

const std::vector<Vertex> vertices = {
    { { -10.0f,  10.0f, 0 }, { 1.0f, 0.0f, 0.0f }, { -1.0f,  1.0f } },
    { {  10.0f,  10.0f, 0 }, { 0.0f, 1.0f, 0.0f }, {  1.0f,  1.0f } },
    { {  10.0f, -10.0f, 0 }, { 0.0f, 0.0f, 1.0f }, {  1.0f, -1.0f } },
    { { -10.0f, -10.0f, 0 }, { 1.0f, 1.0f, 1.0f }, { -1.0f, -1.0f } },
};

const std::vector<uint16_t> indices = {
    0, 1, 2,
    2, 3, 0,
};

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

private:
    #pragma region Objects
    GLFWwindow* window{};

    VkInstance instance{};
    ValidationLayers vLayers;

    PresentationManager presentMan{};
    QueueFamilyIndices _indices;

    VkQueue graphicsQueue{};
    VkQueue presentQueue{};

    VkRenderPass renderPass{};

    VkPipelineLayout pipelineLayout{};
    VkPipeline graphicsPipeline{};

    std::vector<VkCommandPool> commandPools{};

    BufferManager bufferMan;

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
        bufferMan.setUpBufferManager(presentMan.physicalDevice, presentMan.device, instance, vertices, indices, commandPools[0], graphicsQueue);
        createCommandBuffers();
        createSyncObjects();
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
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) const;
    void createSyncObjects();
    void drawFrame();
};
