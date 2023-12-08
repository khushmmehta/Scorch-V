#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <array>
#include <vector>
#include <Abstractions/VulkanMemoryAllocator.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

class BufferManager
{
public:
    VkDescriptorSetLayout descriptorSetLayout{};
    std::vector<VkDescriptorSet> descriptorSets;

    VulkanMemoryAllocator VMA;
    VkBuffer vertexBuffer{};
    VkBuffer indexBuffer{};
    std::vector<VkBuffer> uniformBuffers;
    std::vector<void*> uniformBuffersMapped;

    void createDescriptorSetLayout(VkDevice device);
    void destroyResourceDescriptor(VkDevice device);

    void setUpBufferManager(VkPhysicalDevice physicalDevice, VkDevice device, VkInstance instance, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, VkCommandPool& commandPool, VkQueue gfxQueue);
    void destroyBufferManager();

    void updateUniformBuffers(GLFWwindow* window, uint32_t currentImage);
    void destroyUniformBuffers();

private:
    VmaAllocation vertexBufferAllocation{};
    VmaAllocation indexBufferAllocation{};
    std::vector<VmaAllocation> uniformBuffersAllocation;

    VkDescriptorPool descriptorPool{};

    void createDescriptorPool(VkDevice device);
    void createDescriptorSets(VkDevice device);

    void createVertexArrayObject(VkDevice device, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, VkCommandPool& commandPool, VkQueue gfxQueue);
    void createUniformBuffers();

    template<typename T>
    void createVkBuffer(VkDevice device, const std::vector<T>& data, VkBuffer& buffer, VmaAllocation& bufferAllocation, VkBufferUsageFlags usage, VkCommandPool& commandPool, VkQueue gfxQueue)
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

        VMA.copyBuffer(device, commandPool, gfxQueue, stagingBuffer, buffer, bufferSize);

        vmaFlushAllocation(VMA.allocator, stagingBufferAllocation, 0, data.size());
        vmaDestroyBuffer(VMA.allocator, stagingBuffer, stagingBufferAllocation);
    }
};
