#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

class Shader
{
public:
    uint32_t stageCount = 2;
    VkPipelineShaderStageCreateInfo shaderStages[2] = {};

    Shader(VkDevice& device, const std::string& vert, const std::string& frag);
    void destroyShader(VkDevice& device);

private:
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;

    static VkShaderModule createShaderModule(VkDevice& device, const std::vector<char>& code);
    static std::vector<char> readFile(const std::string& filename);
};
