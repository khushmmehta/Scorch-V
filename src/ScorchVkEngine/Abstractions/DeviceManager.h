#pragma once

#include <vulkan/vulkan.h>
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

class DeviceManager
{
public:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device{};

    QueueFamilyIndices _indices;

    SwapChainSupportDetails swapChainSupport{};
    
    explicit DeviceManager(VkSurfaceKHR& surface)
        : surface(surface)
    {
    }

    void setUpDevices(VkInstance instance, ValidationLayers& vLayers, VkQueue& gfxQ, VkQueue& prstQ)
    {
        pickPhysicalDevice(instance);
        createLogicalDevice(vLayers, gfxQ, prstQ);
    }

    void destroyDevice() { vkDestroyDevice(device, nullptr); }

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice& device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& device);
    static bool checkDeviceExtensionSupport(VkPhysicalDevice& device);

private:
    VkSurfaceKHR& surface;

    bool isDeviceSuitable(VkPhysicalDevice& device);
    void pickPhysicalDevice(VkInstance instance);
    void createLogicalDevice(ValidationLayers vLayers, VkQueue& gfxQ, VkQueue& prstQ);
};
