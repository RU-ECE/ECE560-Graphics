#pragma once

/*
    Based on ogldev_vulkan_core.h by Etay Meiri
    Refactored and Simplified

    There are now standard main and vulkan_init functions.
    They always do the same thing, so there is no reason to write them in each tutorial.alignas

    main is responsible for try... catch, error handling, calling the vulkan_init. This way, later I can refactor openGL to
    do the same thing.

    vulkan_init is responsible for creating the window, initializing GLFW, and calling the render loop.
    It would be nice to share that with OpenGL, but the interaction with Vulkan is different than OpenGL.

    In order to configure the application, there is now a global structure:  app_settings settings;
    Configuring that sets the title, width, height, and background color. Perhaps more attributes later.alignas
    I could definitely see configuring standard defaults like double buffering.
    Perhaps other configurations like standard pipeline setup.

    The other innovation is just to use a global app variable. This way, there is no parameter passing.
    Your only responsibility is to create app = new VulkanApp();
    No question, this dramatically reduces the size and complexity of demo code and it would work for both OpenGL and Vulkan.

    Dov Kruger
    2025-03-11
*/

#include <iostream>
#include <string>
#include <stdio.h>
#include <assert.h>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan/vulkan.h"
#include "ex.hh"

#ifdef _WIN64
#define  VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vk_platform.h"
#else
#define VK_USE_PLATFORM_XCB_KHR
#endif

//TODO: Create a standard multi-lingual exception processing system.
// ex will be non-fatal, fatalex is the fatal one that terminates.
//TODO: #include "ex.hh"

class color {
public:
    float r, g, b;
    color(float r = 0, float g = 0, float b = 0) : r(r), g(g), b(b) {}
    std::ostream& operator<<(std::ostream& os) const {
        os << "(r=" << r << ", g=" << g << ", b=" << b << ")";
        return os;
    }
};

struct app_settings {
	std::string title;
	uint32_t width;
	uint32_t height;
	color bg_color;
    VkApplicationInfo app_info;
};

class vulkan_app_base {
public:
    virtual void init() = 0;
    virtual void render() = 0;
};

#define CHECK_VULKAN_ERROR(msg, res)    \
    if (res != VK_SUCCESS) {            \
        OGLDEV_ERROR(msg, res);         \
        abort();                        \
    }

struct VulkanPhysicalDevices {
    void Init(int num_devices) {
        m_devices.resize(num_devices);
        m_devProps.resize(num_devices);
        m_qFamilyProps.resize(num_devices);
        m_qSupportsPresent.resize(num_devices);
        m_surfaceFormats.resize(num_devices);
        m_surfaceCaps.resize(num_devices);
        m_memProps.resize(num_devices);
    }

    std::vector<VkPhysicalDevice> m_devices;
    std::vector<VkPhysicalDeviceProperties> m_devProps;
    std::vector< std::vector<VkQueueFamilyProperties> > m_qFamilyProps;
    std::vector< std::vector<VkBool32> > m_qSupportsPresent;
    std::vector< std::vector<VkSurfaceFormatKHR> > m_surfaceFormats;
    std::vector<VkSurfaceCapabilitiesKHR> m_surfaceCaps;
    std::vector<VkPhysicalDeviceMemoryProperties> m_memProps;
};

void VulkanEnumExtProps(std::vector<VkExtensionProperties>& ExtProps);
void VulkanPrintImageUsageFlags(const VkImageUsageFlags& flags);
VkShaderModule VulkanCreateShaderModule(VkDevice& device, const char* pFileName);
void VulkanGetPhysicalDevices(const VkInstance& inst, const VkSurfaceKHR& Surface, VulkanPhysicalDevices& PhysDevices);

class VulkanWindowControl {
protected:
    VulkanWindowControl() {};
    ~VulkanWindowControl() {};

public:
    virtual void Init(uint Width, uint Height) = 0;
    virtual VkSurfaceKHR CreateSurface(VkInstance& inst) = 0;
};


class OgldevVulkanCore {
public:
    OgldevVulkanCore(const char* pAppName);
    ~OgldevVulkanCore();
    
    void Init(VulkanWindowControl* pWindowControl);
              
    const VkPhysicalDevice& GetPhysDevice() const;
    
    const VkSurfaceFormatKHR& GetSurfaceFormat() const;
    
    const VkSurfaceCapabilitiesKHR GetSurfaceCaps() const;
    
    const VkSurfaceKHR& GetSurface() const { return m_surface; }
    
    int GetQueueFamily() const { return m_gfxQueueFamily; }
    
    VkInstance& GetInstance() { return m_inst; }
    
    VkDevice& GetDevice() { return m_device; }
    
    uint32_t GetMemoryTypeIndex(uint32_t memTypeBits, VkMemoryPropertyFlags memPropFlags);
    
    VkSemaphore CreateSemaphore();
    
private:
    void CreateInstance();
    void CreateSurface();
    void SelectPhysicalDevice();
    void CreateLogicalDevice();

    // Vulkan objects
    VkInstance m_inst;
    VkSurfaceKHR m_surface;
    VulkanPhysicalDevices m_physDevices;
    VkDevice m_device;
    
    // Internal stuff
    std::string m_appName;
    int m_gfxDevIndex;
    int m_gfxQueueFamily;
};

extern app_settings settings;
extern vulkan_app_base* app;
vulkan_app_base* init_app();