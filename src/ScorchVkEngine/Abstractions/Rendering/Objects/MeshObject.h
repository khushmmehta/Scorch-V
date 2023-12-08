#pragma once

#include <Abstractions/Rendering/BufferManager.h>

class MeshObject
{
public:
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

    void draw(VkCommandBuffer& commandBuffer, VkPipelineLayout pipelineLayout, BufferManager bufferMan, uint32_t currentFrame);
};