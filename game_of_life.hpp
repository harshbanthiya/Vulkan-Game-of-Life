#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <vector>
#include <glm/mat4x4.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600; 

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
        GLFWwindow*         window; 
        VkInstance          instance;
        void initWindow();
        void initVulkan();
        void createInstance();
        void mainLoop();
        void cleanup();
};