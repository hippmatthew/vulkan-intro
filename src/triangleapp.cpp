#include <stdexcept>
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
    if (enableValidationLayers)
    {
        std::cout << "checking validation layer support...\t";
        
        if (!checkValidationLayerSupport())
            throw std::runtime_error("error: validation layers requested, but not available\n");
        
        std::cout << "success\n";
    }
    
    createInstance();
    pickPhysicalDevice();
    createLogicalDevice();
}

void HelloTriangleApplication::mainLoop()
{
    while (!glfwWindowShouldClose(window))
        glfwPollEvents();
}

void HelloTriangleApplication::cleanup()
{
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);
    glfwTerminate(); 
}

bool HelloTriangleApplication::checkValidationLayerSupport()
{
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char * layerName : validationLayers)
    {
        bool layerFound = false;

        for (const auto & layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) return false;
    }

    return true;
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

    bool hasPortabilityExtension = enumerateInstanceExtensions();

    uint32_t glfwExtensionCount = 0;
    const char ** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> requiredExtensions;

    for (uint32_t i = 0; i < glfwExtensionCount; i++)
        requiredExtensions.emplace_back(glfwExtensions[i]);
    
    if (hasPortabilityExtension)
    {
        requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        requiredExtensions.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

        std::cout << "enabled VK_KHR_portability_enumeration extension\n";
    }
    
    createInfo.enabledExtensionCount = (uint32_t) requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
        createInfo.enabledLayerCount = 0;
    
    std::cout << "creating instance...\t";
    
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        throw std::runtime_error("error: failed to create instance");
    
    std::cout << "instance created\n";
}

bool HelloTriangleApplication::enumerateInstanceExtensions()
{
    std::cout << "finding instance extensions...\t";
    
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount); 

    switch (vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data()))
    {
        case VK_SUCCESS:
            std::cout << "all extensions accounted for\n";
            break;
        case VK_INCOMPLETE:
            std::cout << "warning: missing extensions\n";
            break;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            throw std::runtime_error("error: cpu memory limit reached\n");
            break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            throw std::runtime_error("error: gpu memory limit reached\n");
            break;
        default:
            throw std::runtime_error("error: failed to gather instance extensions\n");
    }
    
    std::cout << "available extensions:\n";
    
    bool hasPortabilityExtension = false;
    for (const auto & extension : extensions)
    {
        std::cout << '\t' << extension.extensionName << '\n';
        
        if (strcmp(extension.extensionName, "VK_KHR_portability_enumeration") == 0)
            hasPortabilityExtension = true;
    }
        
    return hasPortabilityExtension; 
}

void HelloTriangleApplication::pickPhysicalDevice()
{
    std::cout << "picking physical device...";
    
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
        throw std::runtime_error("\terror: failed to find GPUs with Vulkan support\n");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    std::cout << "\nsuitable devices:\n";
    for (const auto & device : devices)
    {    
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);

        if (isDeviceSuitable(device))
            std::cout << '\t' << physicalDeviceProperties.deviceName << '\n';
    }
    
    for (const auto & device : devices)
    {
        if (isDeviceSuitable(device))
        {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("error: failed to find a suitable GPU\n");

    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

    std::cout << "chosen device: " << physicalDeviceProperties.deviceName << '\n';
}

bool HelloTriangleApplication::isDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = findQueueFamiles(device);
    
    return indices.isComplete();
}

QueueFamilyIndices HelloTriangleApplication::findQueueFamiles(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    
    int i = 0;
    for (const auto & queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indices.graphicsFamily = i;

        if (indices.isComplete()) break;

        i++;
    }
    
    return indices;
}

void HelloTriangleApplication::createLogicalDevice()
{   
    QueueFamilyIndices indices = findQueueFamiles(physicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    bool hasPortabilityExtension = enumerateDeviceExtensions();
    
    std::vector<const char *> requiredExtensions;
    
    if (hasPortabilityExtension)
    {   
        requiredExtensions.emplace_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
        std::cout << "enabled VK_KHR_portability_subset extension\n";
    }
    
    deviceCreateInfo.enabledExtensionCount = (uint32_t) requiredExtensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
    
    if (enableValidationLayers)
    {
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
        deviceCreateInfo.enabledLayerCount = 0;

    std::cout << "creating logical device...\t";

    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS)
        throw std::runtime_error("error: failed to create logical device\n");

    std::cout << "logical device created\n";
}

bool HelloTriangleApplication::enumerateDeviceExtensions()
{
    std::cout << "finding device extensions...\t";
    
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount); 

    switch (vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions.data()))
    {
        case VK_SUCCESS:
            std::cout << "all extensions accounted for\n";
            break;
        case VK_INCOMPLETE:
            std::cout << "warning: missing extensions\n";
            break;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            throw std::runtime_error("cpu memory limit reached\n");
            break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            throw std::runtime_error("gpu memory limit reached\n");
            break;
        default:
            throw std::runtime_error("failed to gather instance extensions\n");
    }
    
    
    std::cout << "available extensions:\n";
    
    bool hasPortabilityExtension = false;
    for (const auto & extension : extensions)
    {
        std::cout << '\t' << extension.extensionName << '\n';
        
        if (strcmp(extension.extensionName, "VK_KHR_portability_subset") == 0)
            hasPortabilityExtension = true;
    }
        
    return hasPortabilityExtension;

}

