#pragma once

#include <vma/vk_mem_alloc.h>

class VulkanMemoryAllocator
{
public:
    VmaAllocator allocator{};

    void createAllocator(VkPhysicalDevice physicalDevice, VkDevice device, VkInstance instance);

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VmaAllocation& bufferAlloc);

    static void copyBuffer(VkDevice device, VkCommandPool commandPool, VkQueue gfxQueue,
                           VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};
