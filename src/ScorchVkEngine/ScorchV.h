#pragma once
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#include <vector>
#include <optional>
#include <array>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Abstractions/ValidationLayers.h>
#include <Abstractions/PresentationManager.h>
#include <Abstractions/VulkanMemoryAllocator.h>

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

struct Vertex
{
    glm::vec3 pos, color;
    glm::vec2 uv;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescription()
    {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, uv);

        return attributeDescriptions;
    }
};

struct UniformBufferObject
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

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

    VkDescriptorSetLayout descriptorSetLayout{};
    VkPipelineLayout pipelineLayout{};

    VkPipeline graphicsPipeline{};

    std::vector<VkCommandPool> commandPools{};

    VulkanMemoryAllocator VMA;

    VkBuffer vertexBuffer{};
    VmaAllocation vertexBufferMemory{};
    VkBuffer indexBuffer{};
    VmaAllocation indexBufferMemory{};

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VmaAllocation> uniformBuffersAllocation;
    std::vector<void*> uniformBuffersMapped;

    VkDescriptorPool descriptorPool{};
    std::vector<VkDescriptorSet> descriptorSets;

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
        createDescriptorSetLayout();
        createGraphicsPipeline();
        presentMan.createFramebuffers(renderPass);
        createCommandPools();
        VMA.createAllocator(presentMan.physicalDevice, presentMan.device, instance);
        createVertexBuffer();
        createIndexBuffer();
        createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets();
        createCommandBuffers();
        createSyncObjects();
    }

    void mainLoop();

    void cleanup();

    void createInstance();
    void createRenderPass();
    void createDescriptorSetLayout();
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

    template <typename T>
    void createVkBuffer(const std::vector<T>& data, VkBuffer& buffer, VmaAllocation& bufferAllocation, VkBufferUsageFlags usage)
    {
        const VkDeviceSize bufferSize = sizeof(data[0]) * data.size();

        VkBuffer stagingBuffer;
        VmaAllocation stagingBufferAllocation;
        VMA.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferAllocation);

        void* bufferData;
        vmaMapMemory(VMA.allocator, stagingBufferAllocation, &bufferData);
        memcpy(bufferData, data.data(), bufferSize);
        vmaUnmapMemory(VMA.allocator, stagingBufferAllocation);

        VMA.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, bufferAllocation);

        VMA.copyBuffer(presentMan.device, commandPools[0], graphicsQueue, stagingBuffer, buffer, bufferSize);

        vmaFlushAllocation(VMA.allocator, stagingBufferAllocation, 0, data.size());
        vmaDestroyBuffer(VMA.allocator, stagingBuffer, stagingBufferAllocation);
    }

    void createVertexBuffer();
    void createIndexBuffer();
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();

    void createCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) const;
    void createSyncObjects();
    void updateUniformBuffer(uint32_t currentImage);
    void drawFrame();
};
