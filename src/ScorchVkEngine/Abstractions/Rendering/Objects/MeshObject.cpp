#include "MeshObject.h"

void MeshObject::draw(VkCommandBuffer& commandBuffer, VkPipelineLayout pipelineLayout, BufferManager bufferMan, uint32_t currentFrame)
{
    const std::vector<VkBuffer> vertexBuffers = {bufferMan.vertexBuffer};
    constexpr VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(commandBuffer, 0, vertexBuffers.size(), vertexBuffers.data(), offsets);

    vkCmdBindIndexBuffer(commandBuffer, bufferMan.indexBuffer, 0, VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &bufferMan.descriptorSets[currentFrame], 0, nullptr);

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}
