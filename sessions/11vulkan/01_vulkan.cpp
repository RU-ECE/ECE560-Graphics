#include "vulkan_common.hpp"
#include <iostream>
#include <exception>

int main() {
    try {
        VulkanContext ctx = createVulkanFlat("Hello Vulkan", 800, 600, {0.05f, 0.05f, 0.15f, 1.0f});
        ctx.wait();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
