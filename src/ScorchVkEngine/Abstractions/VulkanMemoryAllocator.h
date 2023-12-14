#pragma once

#include <vma/vk_mem_alloc.h>

#include <Abstractions/PresentationManager.h>

class VulkanMemoryAllocator
{
public:
    VmaAllocator allocator{};

    void createAllocator(VkInstance instance);

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VmaAllocation& bufferAlloc);

    void copyBuffer(VkCommandPool commandPool, VkQueue gfxQueue,
                           VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

private:
    PresentationManager* presentMan = PresentationManager::getInstance();
};
