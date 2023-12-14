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
    GLFWwindow* ptrWindow{};
    VkSurfaceKHR surface{};
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device{};

    QueueFamilyIndices _indices;

    VkSwapchainKHR swapChain{};
    VkFormat swapChainImageFormat{};
    VkExtent2D swapChainExtent{};
    std::vector<VkFramebuffer> swapChainFramebuffers;

    bool frameBufferResized = false;

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    void setUpPresentation(VkInstance instance, GLFWwindow* window, ValidationLayers& vLayers, VkQueue& gfxQ, VkQueue& prstQ);
    void destroyPresentation(VkInstance instance);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice& device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& device);
    static bool checkDeviceExtensionSupport(VkPhysicalDevice& device);

    // SwapChain Related
    void cleanupSwapChain();
    void recreateSwapChain(VkRenderPass renderPass);
    void createFramebuffers(VkRenderPass renderPass);

    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

private:

    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;

    SwapChainSupportDetails swapChainSupport{};

    // Device Related
    bool isDeviceSuitable(VkPhysicalDevice& device);
    void pickPhysicalDevice(VkInstance instance);
    void createLogicalDevice(ValidationLayers vLayers, VkQueue& gfxQ, VkQueue& prstQ);

    // SwapChain Related
    void createSwapChain();
    void createImageViews();
};