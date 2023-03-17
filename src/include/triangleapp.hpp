#ifndef triangleapp_hpp
#define triangleapp_hpp

#include <vector>
#include <optional>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#include <GLFW/glfw3.h>

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value();
    }
};

class HelloTriangleApplication
{
    public:
        void run();

    private:
        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
        GLFWwindow * window;

        const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"}; 
        
        #ifdef NDEBUG
            const bool enableValidationLayers = false;
        #else
            const bool enableValidationLayers = true;
        #endif

        VkInstance instance;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;
        
        void initWindow();
        void initVulkan();
        void mainLoop();
        void cleanup();

        void createInstance();
        bool enumerateInstanceExtensions();
        
        bool checkValidationLayerSupport();

        void pickPhysicalDevice();
        bool isDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamiles(VkPhysicalDevice device);

        void createLogicalDevice();
        bool enumerateDeviceExtensions();
};

#endif /* triangleapp_hpp */