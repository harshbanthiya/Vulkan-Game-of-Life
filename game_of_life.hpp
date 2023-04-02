#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <vector>
#include <glm/mat4x4.hpp>
#include <optional>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else 
    const bool enableValidationLayers = true;
#endif

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() {
        return graphicsFamily.has_value();
    }
};


class GameOfLifeApplication
{
    public: 
        void run() 
        {
            initWindow();
            initVulkan();
            mainLoop();
            cleanup();
        }

    private: 
        GLFWwindow*                             window; 
        VkInstance                              instance;
        VkDebugUtilsMessengerEXT                debugMessenger;
        VkPhysicalDevice                        physicalDevice = VK_NULL_HANDLE;
        void                                    initWindow();
        void                                    initVulkan();
        void                                    createInstance();
        void                                    mainLoop();
        void                                    cleanup();
        bool                                    checkValidationLayerSupport();
        std::vector<const char*>                getRequiredExtensions();
        void                                    populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void                                    setupDebugMessenger();
        VkResult                                CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        void                                    DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
        void                                    pickPhysicalDevice();
        bool                                    isDeviceSuitable(VkPhysicalDevice device); 
        QueueFamilyIndices                      findQueueFamilies(VkPhysicalDevice device);
};