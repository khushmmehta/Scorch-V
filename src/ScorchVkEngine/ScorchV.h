#pragma once
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#include <vector>

#include <GLFW/glfw3.h>

#include <Abstractions/ValidationLayers.h>
#include <Abstractions/PresentationManager.h>
#include <Abstractions/Rendering/BufferManager.h>
#include <Abstractions/Rendering/Objects/MeshObject.h>
#include <Abstractions\Rendering\Objects\PhysicsHeader.h>
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

    PresentationManager* presentMan = PresentationManager::getInstance();
    QueueFamilyIndices _indices;

    MeshObject mesh;
    std::vector<RigidBody> rBodies{1};
    std::vector<VertexInstance> vertInstances{1};

    VkQueue graphicsQueue{};
    VkQueue presentQueue{};

    VkRenderPass renderPass{};

    VkPipelineLayout pipelineLayout{};
    VkPipeline graphicsPipeline{};

    std::vector<VkCommandPool> commandPools{};

    BufferManager* bufferMan = BufferManager::getInstance();

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
        presentMan->setUpPresentation(instance, window, vLayers, graphicsQueue, presentQueue);
        createRenderPass();
        bufferMan->createDescriptorSetLayout();
        createGraphicsPipeline();
        presentMan->createFramebuffers(renderPass);
        createCommandPools();
        bufferMan->setUpBufferManager(instance, mesh.vertices, mesh.indices, vertInstances, commandPools[0], graphicsQueue);
        createCommandBuffers();
        createSyncObjects();
        guiMan->setupImGui(instance, window, graphicsQueue, renderPass);
    }

    void mainLoop();
    void cleanup();

    void createInstance();
    void createRenderPass();
    void createGraphicsPipeline();
    void createCommandPools();

    void createVkCommandPool(VkCommandPool& commandPool, VkCommandPoolCreateFlags flags)
    {
        const QueueFamilyIndices queueFamilyIndices = presentMan->findQueueFamilies(presentMan->physicalDevice);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = flags;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(presentMan->device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
            throw std::runtime_error("Failed to create the command pool!");
    }

    void createCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void createSyncObjects();
    void drawFrame();
};
