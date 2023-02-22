#include <stdexcept>
#include <iostream>

#include "include/triangleapp.hpp"

int main()
{
    HelloTriangleApplication app;
    
    try
    {
        app.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}