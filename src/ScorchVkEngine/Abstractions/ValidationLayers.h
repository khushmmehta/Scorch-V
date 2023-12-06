#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include <string>

#ifdef NDEBUG
constexpr bool vLEnabled = false;
#else
constexpr bool vLEnabled = true;
#endif

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

class ValidationLayers {
public:
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    bool enableValidationLayers = vLEnabled;

    void passDebugDataToInstance(VkInstanceCreateInfo& createInfo);
    void setupDebugMessenger(const VkInstance& instance);
    void destroyDebugMessenger(const VkInstance& instance);
    void checkValidationPossible()
    {
        if (enableValidationLayers && !checkValidationLayerSupport())
            throw std::runtime_error("Validation layers requested, but not available!");
    }

private:
    VkDebugUtilsMessengerEXT debugMessenger{};
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

    std::vector<const char*> extensions;

    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT msgSeverity,
        VkDebugUtilsMessageTypeFlagsEXT msgType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);
};
