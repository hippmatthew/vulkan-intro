#ifndef triangleapp_hpp
#define triangleapp_hpp

#include <string>
#include <vector>
#include <optional>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#include <GLFW/glfw3.h>

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class HelloTriangleApplication
{
    public:
        ~HelloTriangleApplication();
        
        void run();

    private:
        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
        GLFWwindow * window;

        const std::vector<const char *> validationLayers = { "VK_LAYER_KHRONOS_validation" };
        const std::vector<const char *> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

        const std::string vertPath = "spv/shader.vert.spv";
        const std::string fragPath = "spv/shader.frag.spv";
        
        #ifdef NDEBUG
            const bool enableValidationLayers = false;
        #else
            const bool enableValidationLayers = true;
        #endif

        VkSurfaceKHR surface;

        VkQueue graphicsQueue;
        VkQueue presentQueue;

        VkInstance instance;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice logicalDevice;
        
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkFramebuffer> swapChainFrameBuffers;

        VkRenderPass renderPass;
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;
        
        VkCommandPool commandPool;
        VkCommandBuffer commandBuffer;
        
        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        VkFence inFlightFence;
        
        void initWindow();
        void initVulkan();
        void mainLoop();

        void createInstance();
        bool enumerateInstanceExtensions();
        
        bool checkValidationLayerSupport();

        void createSurface();
        
        void pickPhysicalDevice();
        bool isDeviceSuitable(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamiles(VkPhysicalDevice device);

        void createLogicalDevice();
        bool enumerateDeviceExtensions();

        void createGraphicsPipeline(const std::string& vertShaderPath, const std::string& fragShaderPath);
        static std::vector<char> readFile(const std::string& filename);
        VkShaderModule createShaderModule(const std::vector<char>& code);

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);          
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes); 
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        void createSwapChain();
        void createImageViews();

        void createRenderPass();
        void createFramebuffers();
        
        void createCommandPool();
        void createCommandBuffer();
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        void drawFrame();
        void createSyncObjects();
};      

#endif /* triangleapp_hpp */