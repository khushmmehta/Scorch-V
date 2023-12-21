#pragma once

#include <glm/glm.hpp>

#include <Abstractions/Rendering/BufferManager.h>

class MeshObject
{
public:
    const std::vector<Vertex> vertices = {
        { { -1.0f,  1.0f, 0 }, { 0.4f, 0.0f, 1.0f }, { -1.0f,  1.0f } },
        { {  1.0f,  1.0f, 0 }, { 0.4f, 0.0f, 1.0f }, {  1.0f,  1.0f } },
        { {  1.0f, -1.0f, 0 }, { 0.4f, 0.0f, 1.0f }, {  1.0f, -1.0f } },
        { { -1.0f, -1.0f, 0 }, { 0.4f, 0.0f, 1.0f }, { -1.0f, -1.0f } },
    };

    const std::vector<uint16_t> indices = {
        0, 1, 2,
        2, 3, 0,
    };

    glm::vec2 pos = {0.0f, 0.0f};

    void draw(VkCommandBuffer& commandBuffer, uint32_t instanceCount, VkPipelineLayout pipelineLayout, uint32_t currentFrame);

private:
    BufferManager* bufferMan = BufferManager::getInstance();
};
