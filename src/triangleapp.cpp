#include <stdexcept>
#include <vector>
#include <iostream>

#include "include/triangleapp.hpp"

void HelloTriangleApplication::run()
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void HelloTriangleApplication::initWindow()
{
    glfwInit();
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void HelloTriangleApplication::initVulkan()
{
    createInstance();
}

void HelloTriangleApplication::mainLoop()
{
    while (!glfwWindowShouldClose(window))
        glfwPollEvents();
}

void HelloTriangleApplication::cleanup()
{
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);
    glfwTerminate();
}

void HelloTriangleApplication::createInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    validateExtensions();

    uint32_t glfwExtensionCount = 0;
    const char ** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> requiredExtensions;

    for (uint32_t i = 0; i < glfwExtensionCount; i++)
        requiredExtensions.emplace_back(glfwExtensions[i]);
    requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    createInfo.enabledExtensionCount = (uint32_t) requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

    createInfo.enabledLayerCount = 0;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        throw std::runtime_error("failed to create instance");
}

void HelloTriangleApplication::validateExtensions()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount); 

    switch (vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data()))
    {
        case VK_SUCCESS:
            std::cout << "all extensions accounted for\n";
            break;
        case VK_INCOMPLETE:
            std::cout << "warning: missing extensions";
            break;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            throw std::runtime_error("out of host memory");
            break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            throw std::runtime_error("out of device memory");
            break;
        default:
            throw std::runtime_error("failed to gather instance extensions");
    }
    
    std::cout << "available extensions:\n";
    for (const auto & extension : extensions)
        std::cout << '\t' << extension.extensionName << '\n';
   
}