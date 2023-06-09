#include "game_of_life.hpp"

void GameOfLifeApplication::initWindow()
{
    glfwInit();  // Initialize glfw 
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Since this OPENGL, we need to tell glfw 
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Currently disabled resizing the window
    
    window = glfwCreateWindow(WIDTH, HEIGHT, "Game of Life", nullptr, nullptr);
}

void  GameOfLifeApplication::createInstance() 
{
    if (enableValidationLayers && !checkValidationLayerSupport())
        throw std::runtime_error("Validation layers requested, but not available");
    
    VkApplicationInfo       app_info = {};

    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Game of Life";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo  create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    auto extensions = getRequiredExtensions();
    
    create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR; // Add bit to flags
    create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    create_info.ppEnabledExtensionNames = extensions.data();

    // Debug Struct
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers)
    {
        create_info.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        create_info.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
    }
    else 
    {
        create_info.enabledLayerCount = 0;
        create_info.pNext = nullptr;
    }

    VkResult result = vkCreateInstance(&create_info, nullptr, &instance);

    if (result != VK_SUCCESS)
        throw std::runtime_error("Failed to create instance!");
}

void GameOfLifeApplication::initVulkan() 
{
    createInstance();
    setupDebugMessenger();
    pickPhysicalDevice();
    createLogicalDevice();
}

void GameOfLifeApplication::mainLoop() 
{
    while (!glfwWindowShouldClose(window))
        glfwPollEvents();
}

void GameOfLifeApplication::cleanup()
{
    vkDestroyDevice(device, nullptr);
    if (enableValidationLayers) 
       DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool GameOfLifeApplication::isDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = findQueueFamilies(device);
    return indices.isComplete();
}

void GameOfLifeApplication::pickPhysicalDevice()
{
    // Lets list all graphics card and query them; although in this implementation we will use the first one
    uint32_t    device_count = 0; 
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    if (device_count == 0) // 0 devices with vulkan support no point going further
        throw std::runtime_error("failed to find GPUs with Vulkan Support");
    std::vector<VkPhysicalDevice> devices (device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    // Now lets evaluate each of them and see if they are suitable for operations we want to perform
    for (const auto& device : devices)
    {
        if (isDeviceSuitable(device))
        {
            physicalDevice = device;
            break ;
        }
    }
    if (physicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("Failed to find a suitable GPU");
}

QueueFamilyIndices GameOfLifeApplication::findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices      indices;

    // just like other things in Vulkan, it follows the same flow 
    uint32_t                queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily: queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) // We need to find atleast one family that supports the Graphics bit 
            indices.graphicsFamily = i;
        if (indices.isComplete())
            break ;
        i++;
    }
    return indices;
}

void  GameOfLifeApplication::createLogicalDevice() // LOGICAL Device is the interface to the physical device
{
    QueueFamilyIndices  indices  = findQueueFamilies(physicalDevice);

    // VkDevice Create info struct to describe number of queues we want for single queue family ; rn it is graphical only
    VkDeviceQueueCreateInfo  queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    // Vulkan lets you assign priorities to queues to influence the scheduling of command buffer execution using floating point numbers between 0.0 and 1.0. 
    // This is required even if there is only a single queue:
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures    deviceFeatures{}; // vkGetPhysicalDeviceFeatures will get specific features like geometry shaders and stuff rn we dont need it 
    VkDeviceCreateInfo          createInfo{}; // Create logical device

    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;

    if (enableValidationLayers) 
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else 
        createInfo.enabledLayerCount = 0;
    
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) 
        throw std::runtime_error("Failed to create logical device!");

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
}