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

    uint32_t glfwExtensionCount = 0;
    const char ** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    // Mac might have incompatible drivers 
    std::vector<const char *> requiredExtensions;

    for (uint32_t i = 0; i < glfwExtensionCount; i++)
        requiredExtensions.__emplace_back(glfwExtensions[i]);
    requiredExtensions.__emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME); // Add instace enabled extension name
    create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR; // Add bit to flags

    create_info.enabledExtensionCount = (uint32_t) requiredExtensions.size();
    create_info.ppEnabledExtensionNames = requiredExtensions.data();
    create_info.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&create_info, nullptr, &instance);

    if (result != VK_SUCCESS)
        throw std::runtime_error("Failed to create instance!");
}

void GameOfLifeApplication::initVulkan() 
{
    createInstance();
}


void GameOfLifeApplication::mainLoop() 
{
    while (!glfwWindowShouldClose(window))
        glfwPollEvents();
}
void GameOfLifeApplication::cleanup()
{
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}