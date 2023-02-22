#ifndef triangleapp_hpp
#define triangleapp_hpp

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class HelloTriangleApplication
{
    public:
        void run();

    private:
        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
        GLFWwindow * window;

        VkInstance instance;
        
        void initWindow();
        void initVulkan();
        void mainLoop();
        void cleanup();

        void createInstance();
        void validateExtensions();

};

#endif /* triangleapp_hpp */