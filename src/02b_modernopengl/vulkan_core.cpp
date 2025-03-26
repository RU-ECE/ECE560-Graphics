/*
    Vulkan core utilities, based on a fusion of Etay Meiri's Vulkan tutorial (ogldev.org)
    and the Vulkan Examples by Sascha Willems (saschawillems.de).

    Author: Dov Kruger
    Date: 2025-03-11
*/

#include "vulkan_core.hh"
#include <queue>
#include <fstream>
#include <vulkan/vulkan.h>

using namespace std;

language current_lang = language::en;

void test_all_exceptions(ofstream& log) {
    // Cast to underlying type to allow iteration
    for (int lang = static_cast<int>(language::en); 
         lang <= static_cast<int>(language::tr); 
         lang++) {
        set_language(static_cast<language>(lang));
        for (int err = static_cast<int>(error_type::out_of_memory); 
             err <= static_cast<int>(error_type::file_not_a_regular_file); 
             err++) {
            try {
                throw ex(__FILE__, __LINE__, static_cast<error_type>(err), "test");
            } catch (const ex& e) {
                log << e << endl;
            }
        }
    }
}

// Verify all enum values are present exactly once
constexpr bool verify_mappings(ofstream& log) {
    bool seen[static_cast<size_t>(error_type::vk_enumerate_devices) + 1] = {};
    
    // Check for duplicates
    for (const auto& m : error_strings) {
        size_t index = static_cast<size_t>(m.err);
        if (index >= sizeof(seen)/sizeof(seen[0])) {
            log << "Error index " << index << " out of bounds\n";
            return false;
        }
        if (seen[index]) {
            log << "Duplicate error mapping for index " << index << "\n";
            return false;
        }
        seen[index] = true;
    }

    // Check for missing mappings
    for (size_t i = 0; i < sizeof(seen)/sizeof(seen[0]); ++i) {
        if (!seen[i]) {
            cerr << "Missing error mapping for index " << i << "\n";
            return false;
        }
    }
    return true;
}


PFN_vkCreateDebugReportCallbackEXT my_vkCreateDebugReportCallbackEXT = NULL;


VKAPI_ATTR VkBool32 VKAPI_CALL MyDebugReportCallback(
    VkDebugReportFlagsEXT       flags,
    VkDebugReportObjectTypeEXT  objectType,
    uint64_t                    object,
    size_t                      location,
    int32_t                     messageCode,
    const char*                 pLayerPrefix,
    const char*                 pMessage,
    void*                       pUserData)
{
    printf("%s\n", pMessage);
    return VK_FALSE;
}




OgldevVulkanCore::OgldevVulkanCore(const char* pAppName)
{
    m_appName = std::string(pAppName);
    m_gfxDevIndex = -1;
    m_gfxQueueFamily = -1;
}


OgldevVulkanCore::~OgldevVulkanCore()
{

}


void OgldevVulkanCore::Init(VulkanWindowControl* pWindowControl)
{
    std::vector<VkExtensionProperties> ExtProps;
    VulkanEnumExtProps(ExtProps);

    CreateInstance();

    m_surface = pWindowControl->CreateSurface(m_inst);
    assert(m_surface);

    printf("Surface created\n");

    VulkanGetPhysicalDevices(m_inst, m_surface, m_physDevices);
    SelectPhysicalDevice();
    CreateLogicalDevice();
}

const VkPhysicalDevice& OgldevVulkanCore::GetPhysDevice() const
{
    assert(m_gfxDevIndex >= 0);
    return m_physDevices.m_devices[m_gfxDevIndex];
}

const VkSurfaceFormatKHR& OgldevVulkanCore::GetSurfaceFormat() const
{
    assert(m_gfxDevIndex >= 0);
    return m_physDevices.m_surfaceFormats[m_gfxDevIndex][0];
}


const VkSurfaceCapabilitiesKHR OgldevVulkanCore::GetSurfaceCaps() const
{
    assert(m_gfxDevIndex >= 0);
    return m_physDevices.m_surfaceCaps[m_gfxDevIndex];

}


void OgldevVulkanCore::SelectPhysicalDevice()
{
    for (uint i = 0 ; i < m_physDevices.m_devices.size() ; i++) {

        for (uint j = 0 ; j < m_physDevices.m_qFamilyProps[i].size() ; j++) {
            VkQueueFamilyProperties& QFamilyProp = m_physDevices.m_qFamilyProps[i][j];

            printf("Family %d Num queues: %d\n", j, QFamilyProp.queueCount);
            VkQueueFlags flags = QFamilyProp.queueFlags;
            printf("    GFX %s, Compute %s, Transfer %s, Sparse binding %s\n",
                    (flags & VK_QUEUE_GRAPHICS_BIT) ? "Yes" : "No",
                    (flags & VK_QUEUE_COMPUTE_BIT) ? "Yes" : "No",
                    (flags & VK_QUEUE_TRANSFER_BIT) ? "Yes" : "No",
                    (flags & VK_QUEUE_SPARSE_BINDING_BIT) ? "Yes" : "No");

            if ((flags & VK_QUEUE_GRAPHICS_BIT) && (m_gfxDevIndex == -1)) {
                if (!m_physDevices.m_qSupportsPresent[i][j]) {
                    printf("Present is not supported\n");
                    continue;
                }

                m_gfxDevIndex = i;
                m_gfxQueueFamily = j;
                printf("Using GFX device %d and queue family %d\n", m_gfxDevIndex, m_gfxQueueFamily);
            }
        }
    }

    if (m_gfxDevIndex == -1) {
        printf("No GFX device found!\n");
        assert(0);
    }
}


void OgldevVulkanCore::CreateInstance()
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = m_appName.c_str();
    appInfo.engineVersion = 1;
    appInfo.apiVersion = VK_API_VERSION_1_0;

    const char* pInstExt[] = {
#ifdef ENABLE_DEBUG_LAYERS
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
#endif
        VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef _WIN32
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#else
     //   VK_KHR_XCB_SURFACE_EXTENSION_NAME
#endif
    };

#ifdef ENABLE_DEBUG_LAYERS
    const char* pInstLayers[] = {
        "VK_LAYER_LUNARG_standard_validation"
    };
#endif

    VkInstanceCreateInfo instInfo = {};
    instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pApplicationInfo = &appInfo;
#ifdef ENABLE_DEBUG_LAYERS
    instInfo.enabledLayerCount = sizeof(pInstLayers)/sizeof(pInstLayers[0]);
    instInfo.ppEnabledLayerNames = pInstLayers;
#endif
    instInfo.enabledExtensionCount = sizeof(pInstExt)/sizeof(pInstExt[0]);
    instInfo.ppEnabledExtensionNames = pInstExt;

    VkResult res = vkCreateInstance(&instInfo, NULL, &m_inst);
    vkcheck_msg(res, error_type::initialization, "vkCreateInstance");

#ifdef ENABLE_DEBUG_LAYERS
    // Get the address to the vkCreateDebugReportCallbackEXT function
    my_vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(m_inst, "vkCreateDebugReportCallbackEXT"));

    // Register the debug callback
    VkDebugReportCallbackCreateInfoEXT callbackCreateInfo;
    callbackCreateInfo.sType       = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    callbackCreateInfo.pNext       = NULL;
    callbackCreateInfo.flags       = VK_DEBUG_REPORT_ERROR_BIT_EXT |
                                     VK_DEBUG_REPORT_WARNING_BIT_EXT |
                                     VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
    callbackCreateInfo.pfnCallback = &MyDebugReportCallback;
    callbackCreateInfo.pUserData   = NULL;

    VkDebugReportCallbackEXT callback;
    res = my_vkCreateDebugReportCallbackEXT(m_inst, &callbackCreateInfo, NULL, &callback);
    CHECK_VULKAN_ERROR("my_vkCreateDebugReportCallbackEXT error %d\n", res);
#endif
}


void create_LogicalDevice(VkDevice& device, VkPhysicalDevice& physDevice, uint32_t gfxQueueFamily) {
    VkDeviceQueueCreateInfo qInfo = {};
    qInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

    float qPriorities = 1.0f;
    qInfo.queueCount = 1;
    qInfo.pQueuePriorities = &qPriorities;
    qInfo.queueFamilyIndex = gfxQueueFamily;

    const char* pDevExt[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDeviceCreateInfo devInfo = {};
    devInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    devInfo.enabledExtensionCount = sizeof(pDevExt)/sizeof(pDevExt[0]);
    devInfo.ppEnabledExtensionNames = pDevExt;
    devInfo.queueCreateInfoCount = 1;
    devInfo.pQueueCreateInfos = &qInfo;

    VkResult res = vkCreateDevice(physDevice, &devInfo, NULL, &device);
    vkcheck(res, error_type::vk_create_device);
}

void create_vertex_buffer(VkDevice& device, VkBuffer& vertexBuffer, VkBuffer& indexBuffer,
                            const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
    uint32_t vertexBufferSize = static_cast<uint32_t>(vertices.size()) * sizeof(Vertex);
    uint32_t index_count = static_cast<uint32_t>(indices.size());
    uint32_t indexBufferSize = indexCount * sizeof(uint32_t);

    VkMemoryAllocateInfo memAlloc{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
    VkMemoryRequirements memReqs;

    // Static data like vertex and index buffer should be stored on the device memory for optimal (and fastest) access by the GPU
    //
    // To achieve this we use so-called "staging buffers" :
    // - Create a buffer that's visible to the host (and can be mapped)
    // - Copy the data to this buffer
    // - Create another buffer that's local on the device (VRAM) with the same size
    // - Copy the data from the host to the device using a command buffer
    // - Delete the host visible (staging) buffer
    // - Use the device local buffers for rendering
    //
    // Note: On unified memory architectures where host (CPU) and GPU share the same memory, staging is not necessary
    // To keep this sample easy to follow, there is no check for that in place

    // Create the host visible staging buffer that we copy vertices and indices too, and from which we copy to the device
    VulkanBuffer stagingBuffer;
    VkBufferCreateInfo stagingBufferCI{ VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
    stagingBufferCI.size = vertexBufferSize + indexBufferSize;
    // Buffer is used as the copy source
    stagingBufferCI.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    // Create a host-visible buffer to copy the vertex data to (staging buffer)
    vkcheck_msg(vkCreateBuffer(device, &stagingBufferCI, nullptr, &stagingBuffer.handle), error_type::initialization, "vkCreateBuffer");
    vkGetBufferMemoryRequirements(device, stagingBuffer.handle, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    // Request a host visible memory type that can be used to copy our data to
    // Also request it to be coherent, so that writes are visible to the GPU right after unmapping the buffer
    memAlloc.memoryTypeIndex = getMemoryTypeIndex(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    vkcheck_msg(vkAllocateMemory(device, &memAlloc, nullptr, &stagingBuffer.memory), error_type::initialization, "vkAllocateMemory");
    vkcheck_msg(vkBindBufferMemory(device, stagingBuffer.handle, stagingBuffer.memory, 0), error_type::initialization, "vkBindBufferMemory");
    // Map the buffer and copy vertices and indices into it, this way we can use a single buffer as the source for both vertex and index GPU buffers
    uint8_t* data{ nullptr };
    vkcheck_msg(vkMapMemory(device, stagingBuffer.memory, 0, memAlloc.allocationSize, 0, (void**)&data), error_type::initialization, "vkMapMemory");
    memcpy(data, vertices.data(), vertexBufferSize);
    memcpy(((char*)data) + vertexBufferSize, indices.data(), vertexBufferSize);

    // Create a device local buffer to which the (host local) vertex data will be copied and which will be used for rendering
    VkBufferCreateInfo vertexbufferCI{ VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
    vertexbufferCI.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    vertexbufferCI.size = vertexBufferSize;
    vkcheck_msg(vkCreateBuffer(device, &vertexbufferCI, nullptr, &vertexBuffer.handle), error_type::initialization, "vkCreateBuffer");
    vkGetBufferMemoryRequirements(device, vertexBuffer.handle, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    memAlloc.memoryTypeIndex = getMemoryTypeIndex(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    vkcheck_msg(vkAllocateMemory(device, &memAlloc, nullptr, &vertexBuffer.memory), error_type::initialization, "vkAllocateMemory");
    vkcheck_msg(vkBindBufferMemory(device, vertexBuffer.handle, vertexBuffer.memory, 0), error_type::initialization, "vkBindBufferMemory");

    // Create a device local buffer to which the (host local) index data will be copied and which will be used for rendering
    VkBufferCreateInfo indexbufferCI{ VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
    indexbufferCI.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    indexbufferCI.size = indexBufferSize;
    vkcheck_msg(vkCreateBuffer(device, &indexbufferCI, nullptr, &indexBuffer.handle), error_type::initialization, "vkCreateBuffer");
    vkGetBufferMemoryRequirements(device, indexBuffer.handle, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    memAlloc.memoryTypeIndex = getMemoryTypeIndex(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    vkcheck_msg(vkAllocateMemory(device, &memAlloc, nullptr, &indexBuffer.memory), error_type::initialization, "vkAllocateMemory");
    vkcheck_msg(vkBindBufferMemory(device, indexBuffer.handle, indexBuffer.memory, 0), error_type::initialization, "vkBindBufferMemory");

    // Buffer copies have to be submitted to a queue, so we need a command buffer for them
    VkCommandBuffer copyCmd;

    VkCommandBufferAllocateInfo cmdBufAllocateInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
    cmdBufAllocateInfo.commandPool = commandPool;
    cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBufAllocateInfo.commandBufferCount = 1;
    vkcheck_msg(vkAllocateCommandBuffers(device, &cmdBufAllocateInfo, &copyCmd), error_type::initialization, "vkAllocateCommandBuffers");

    VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();
    vkcheck_msg(vkBeginCommandBuffer(copyCmd, &cmdBufInfo), error_type::initialization, "vkBeginCommandBuffer");
    // Copy vertex and index buffers to the device
    VkBufferCopy copyRegion{};
    copyRegion.size = vertexBufferSize;
    vkCmdCopyBuffer(copyCmd, stagingBuffer.handle, vertexBuffer.handle, 1, &copyRegion);
    copyRegion.size = indexBufferSize;
    // Indices are stored after the vertices in the source buffer, so we need to add an offset
    copyRegion.srcOffset = vertexBufferSize;
    vkCmdCopyBuffer(copyCmd, stagingBuffer.handle, indexBuffer.handle,	1, &copyRegion);
    vkcheck_msg(vkEndCommandBuffer(copyCmd), error_type::initialization, "vkEndCommandBuffer");

    // Submit the command buffer to the queue to finish the copy
    VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &copyCmd;

    // Create fence to ensure that the command buffer has finished executing
    VkFenceCreateInfo fenceCI{ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
    VkFence fence;
    vkcheck_msg(vkCreateFence(device, &fenceCI, nullptr, &fence), error_type::initialization, "vkCreateFence");
    // Submit copies to the queue
    vkcheck_msg(vkQueueSubmit(queue, 1, &submitInfo, fence), error_type::initialization, "vkQueueSubmit");
    // Wait for the fence to signal that command buffer has finished executing
    vkcheck_msg(vkWaitForFences(device, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT), error_type::initialization, "vkWaitForFences");
    vkDestroyFence(device, fence, nullptr);
    vkFreeCommandBuffers(device, commandPool, 1, &copyCmd);

    // The fence made sure copies are finished, so we can safely delete the staging buffer
    vkDestroyBuffer(device, stagingBuffer.handle, nullptr);
    vkFreeMemory(device, stagingBuffer.memory, nullptr);
}

#if 0
uint32_t get_memory_type_index(uint32_t memTypeBits, VkMemoryPropertyFlags reqMemPropFlags) {
    const VkPhysicalDeviceMemoryProperties& physDeviceMemProps = m_physDevices.m_memProps[m_gfxDevIndex];
    for (uint i = 0 ; i < physDeviceMemProps.memoryTypeCount ; i++) {
        if ((memTypeBits & (1 << i)) &&
            ((physDeviceMemProps.memoryTypes[i].propertyFlags & reqMemPropFlags) == reqMemPropFlags)) {
            return i;
        }
    }

    vk_check(res, error_type::vk_get_memory_type_index, memTypeBits, reqMemPropFlags);
}
#endif

VkSemaphore create_semaphore(VkDevice device) {
    VkSemaphoreCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkSemaphore semaphore;
    VkResult res = vkCreateSemaphore(device, &createInfo, NULL, &semaphore);
    vkcheck_msg(res, error_type::initialization, "vkCreateSemaphore");
    return semaphore;
}


void physical_device_init(VkInstance instance) {
    // Physical device
	uint32_t gpuCount = 0;
	// Get number of available physical devices
	vkcheck(vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr), error_type::initialization, "vkEnumeratePhysicalDevices");
	// Enumerate devices
	std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
	uint32_t result = vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data());
	vkcheck_msg(result, error_type::initialization, "vkEnumeratePhysicalDevices");
}

void vulkan_init() {
    // Create application info
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = settings.title.c_str();
    appInfo.engineVersion = 1;
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Create instance info
    VkInstanceCreateInfo instInfo = {};
    instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pApplicationInfo = &appInfo;

    // Create the instance
    VkInstance instance;
    VkResult res = vkCreateInstance(&instInfo, nullptr, &instance);
    vkcheck_msg(res, error_type::initialization, "vkCreateInstance");

    physical_device_init(instance);
}

bool input_verbose = false;
//TODO: This should move into utilities with programmable key bindings
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
	if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
		glfwSetWindowShouldClose(win, GLFW_TRUE);
	}
    if (input_verbose)
    cout << "key " << key << " " << scancode << " " << action << " " << mods << endl;
}

void cursor_position_callback(GLFWwindow* win, double xpos, double ypos) {
    if (input_verbose)
        cout << "mouse position " << xpos << " " << ypos << endl;
}

void mouse_callback(GLFWwindow* win, int button, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(win, &xpos, &ypos);
    if (input_verbose)
        cout << "mouse " << button << " " << action << " " << mods 
             << " at " << xpos << "," << ypos << endl;
}

void window_init() {
	if (!glfwInit()) {
		throw "Failed to initialize GLFW";
	}

	if (!glfwVulkanSupported()) {
		throw "GLFW does not support Vulkan";
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* win = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), nullptr, nullptr);

    vulkan_init();
    app = init_app();
	//m_vkCore.Init(title, nullptr, false);  // The NULL is for forward compatibility with the following tutorials
	if (win == nullptr) throw "Failed openging window";
	glfwSetKeyCallback(win, key_callback);
    glfwSetMouseButtonCallback(win, mouse_callback);
    glfwSetCursorPosCallback(win, cursor_position_callback);
    try {
            app->init();
    } catch (const ex& e) {
        cerr << e << endl;
    }
    while (!glfwWindowShouldClose(win)) {
        try {
                app->render();
        } catch (const ex& e) {
            cerr << e << endl;
        }
        // TODO: add fatalex to break out.
		glfwPollEvents();
	}

	glfwTerminate();
}

void unit_tests() {
    ofstream log("unit_tests.log");
    if (!verify_mappings(log))
        log << "Error mapping is incomplete or has duplicates";
    test_all_exceptions(log);
}

int main(int argc, char* argv[]) {
    unit_tests();
    set_language(language::en);
	try {
		window_init();
	} catch (const char* msg) {
		cerr << msg << endl;
	}
	return 0;
}

void vulkan_app_base::init() {
}

void vulkan_app_base::render() {
}

// Add these implementations
void VulkanEnumExtProps(std::vector<VkExtensionProperties>& ExtProps) {
    uint32_t ExtCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &ExtCount, nullptr);
    ExtProps.resize(ExtCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &ExtCount, ExtProps.data());
}

void VulkanGetPhysicalDevices(const VkInstance& inst, const VkSurfaceKHR& Surface, VulkanPhysicalDevices& PhysDevices) {
    uint32_t DeviceCount = 0;
    vkEnumeratePhysicalDevices(inst, &DeviceCount, nullptr);
    PhysDevices.Init(DeviceCount);
    vkEnumeratePhysicalDevices(inst, &DeviceCount, PhysDevices.m_devices.data());
    // ... populate other device properties ...
}

// Define the global variables
vulkan_app_base* app = nullptr;
