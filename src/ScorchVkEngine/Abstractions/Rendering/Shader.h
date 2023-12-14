#pragma once

#include <vulkan/vulkan.h>

#include <string>
#include <vector>

#include <Abstractions/PresentationManager.h>

class Shader
{
public:
    uint32_t stageCount = 2;
    VkPipelineShaderStageCreateInfo shaderStages[2] = {};

    Shader(const std::string& vert, const std::string& frag);
    void destroyShader();

private:
    PresentationManager* presentMan = PresentationManager::getInstance();

    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;

    VkShaderModule createShaderModule(const std::vector<char>& code);
    static std::vector<char> readFile(const std::string& filename);
};
