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
}

void GameOfLifeApplication::mainLoop() 
{
    while (!glfwWindowShouldClose(window))
        glfwPollEvents();
}

void GameOfLifeApplication::cleanup()
{
    if (enableValidationLayers) 
       DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}

