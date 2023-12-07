#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>
#include <vector>

#include <Abstractions/ValidationLayers.h>

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class PresentationManager
{
public:
    VkSurfaceKHR surface{};
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device{};

    QueueFamilyIndices _indices;

    SwapChainSupportDetails swapChainSupport{};

    void setUpPresentation(VkInstance instance, GLFWwindow* window, ValidationLayers& vLayers, VkQueue& gfxQ, VkQueue& prstQ);
    void destroyPresentation(VkInstance instance);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice& device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& device);
    static bool checkDeviceExtensionSupport(VkPhysicalDevice& device);

private:

    bool isDeviceSuitable(VkPhysicalDevice& device);
    void pickPhysicalDevice(VkInstance instance);
    void createLogicalDevice(ValidationLayers vLayers, VkQueue& gfxQ, VkQueue& prstQ);
};
