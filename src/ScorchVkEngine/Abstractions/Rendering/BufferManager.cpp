#define MAX_FRAMES_IN_FLIGHT 2
#include "BufferManager.h"

#include <stdexcept>

void BufferManager::createDescriptorSetLayout(VkDevice device)
{
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
        throw std::runtime_error("Failed to create a descriptor set layout!");
}

void BufferManager::destroyResourceDescriptor(VkDevice device)
{
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
}

void BufferManager::createDescriptorPool(VkDevice device)
{
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
        throw std::runtime_error("Failed to create a descriptor pool!");
}

void BufferManager::createDescriptorSets(VkDevice device)
{
    const std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS)
        throw std::runtime_error("Failed to allocate descriptor sets!");

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
    }
}

void BufferManager::setUpBufferManager(VkPhysicalDevice physicalDevice, VkDevice device, VkInstance instance,
    const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, VkCommandPool& commandPool, VkQueue gfxQueue)
{
    VMA.createAllocator(physicalDevice, device, instance);
    createVertexArrayObject(device, vertices, indices, commandPool, gfxQueue);
    createUniformBuffers();

    createDescriptorPool(device);
    createDescriptorSets(device);
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

void BufferManager::updateUniformBuffers(GLFWwindow* window, uint32_t currentImage)
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
