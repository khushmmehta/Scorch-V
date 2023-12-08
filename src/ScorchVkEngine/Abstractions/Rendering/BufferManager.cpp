#define MAX_FRAMES_IN_FLIGHT 2
#include "BufferManager.h"

void BufferManager::setUpBufferManager(VkPhysicalDevice physicalDevice, VkDevice device, VkInstance instance,
    const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, VkCommandPool& commandPool, VkQueue gfxQueue)
{
    VMA.createAllocator(physicalDevice, device, instance);
    createVertexArrayObject(device, vertices, indices, commandPool, gfxQueue);
    createUniformBuffers();
}

void BufferManager::destroyBufferManager()
{
    vmaDestroyBuffer(VMA.allocator, indexBuffer, indexBufferAllocation);
    vmaDestroyBuffer(VMA.allocator, vertexBuffer, vertexBufferAllocation);

    vmaDestroyAllocator(VMA.allocator);
}

void BufferManager::createVertexArrayObject(VkDevice device, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, VkCommandPool& commandPool, VkQueue gfxQueue)
{
    createVkBuffer<Vertex>(device, vertices, vertexBuffer, vertexBufferAllocation, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, commandPool, gfxQueue);
    createVkBuffer<uint16_t>(device, indices, indexBuffer, indexBufferAllocation, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, commandPool, gfxQueue);
}

void BufferManager::createUniformBuffers()
{
    constexpr VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersAllocation.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VMA.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersAllocation[i]);
        vmaMapMemory(VMA.allocator, uniformBuffersAllocation[i], &uniformBuffersMapped[i]);
    }
}

void BufferManager::updateUniformBuffer(GLFWwindow* window, uint32_t currentImage)
{
    UniformBufferObject ubo{};
    ubo.model = translate(glm::mat4(1.0f), glm::vec3(0.0f));
    ubo.view = translate(glm::mat4(1.0f), glm::vec3(0.0f));

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    const float windowHalfWidth = width * 0.5f;
    const float windowHalfHeight = height * 0.5f;

    ubo.proj = glm::ortho(-windowHalfWidth / 10, windowHalfWidth / 10, -windowHalfHeight / 10, windowHalfHeight / 10, -1.0f, 1.0f);
    ubo.proj[1][1] *= -1;

    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void BufferManager::destroyUniformBuffers()
{
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vmaUnmapMemory(VMA.allocator, uniformBuffersAllocation[i]);
        vmaDestroyBuffer(VMA.allocator, uniformBuffers[i], uniformBuffersAllocation[i]);
    }
}
