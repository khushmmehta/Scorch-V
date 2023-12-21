#include "MeshObject.h"

void MeshObject::draw(VkCommandBuffer& commandBuffer, uint32_t instanceCount, VkPipelineLayout pipelineLayout,uint32_t currentFrame)
{
    const std::vector<VkBuffer> vertexBuffers = { bufferMan->vertexBuffer };
    const std::vector<VkBuffer> instanceBuffers = { bufferMan->instanceBuffer };
    constexpr VkDeviceSize offsets[] = {0, 0};

    vkCmdBindVertexBuffers(commandBuffer, 0, vertexBuffers.size(), vertexBuffers.data(), offsets);
    vkCmdBindVertexBuffers(commandBuffer, 1, instanceBuffers.size(), instanceBuffers.data(), offsets);

    vkCmdBindIndexBuffer(commandBuffer, bufferMan->indexBuffer, 0, VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &bufferMan->descriptorSets[currentFrame], 0, nullptr);

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), instanceCount, 0, 0, 0);
}
