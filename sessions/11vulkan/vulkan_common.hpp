#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <array>
#include <functional>
#include <vector>

struct color { float r, g, b, a; };

// Base Vulkan context: instance, device, swapchain, color-only render pass,
// framebuffers, pre-recorded clear command buffers, and sync objects.
// No depth buffer — suitable for 2D/flat drawing or as a base for derived contexts.
struct VulkanContext {
    GLFWwindow*         win         = nullptr;
    vk::Instance        instance;
    vk::SurfaceKHR      surface;
    vk::PhysicalDevice  physDev;
    vk::Device          device;
    vk::Queue           queue;
    uint32_t            qFamilyIdx  = 0;

    vk::SurfaceFormatKHR  surfaceFmt;
    vk::Extent2D          extent;
    vk::SwapchainKHR      swapchain;
    std::vector<vk::Image>       swapImages;
    std::vector<vk::ImageView>   imageViews;

    vk::RenderPass                renderPass;
    std::vector<vk::Framebuffer>  framebuffers;

    vk::CommandPool                 cmdPool;
    std::vector<vk::CommandBuffer>  cmds;

    static constexpr uint32_t MAX_FRAMES = 2;
    std::vector<vk::Semaphore>  imgAvail;
    std::vector<vk::Semaphore>  renderDone;
    std::vector<vk::Fence>      inFlight;
    std::vector<vk::Fence>      imgFences;
    uint32_t frame = 0;

    // ── Drawing ──────────────────────────────────────────────────────────────
    struct ShaderStage {
        vk::ShaderModule        module;
        vk::ShaderStageFlagBits stage;
    };
    // Staging area: compile shaders here, then call buildFlatPipeline().
    // Clear manually between pipeline builds if reusing the context.
    std::vector<ShaderStage>    shaders;
    // Owned pipelines/layouts — destroyed in destructor
    std::vector<vk::Pipeline>           pipelines;
    std::vector<vk::PipelineLayout>     pipelineLayouts;

    // ── GPU buffers ───────────────────────────────────────────────────────────
    struct GpuBuffer { vk::Buffer buffer; vk::DeviceMemory memory; };
    std::vector<GpuBuffer> buffers;  // owned; destroyed in destructor

    // Upload to device-local memory via a staging buffer.  The buffer is
    // stored in buffers[] for automatic cleanup; return value is for binding.
    GpuBuffer uploadVertices(const void* data, vk::DeviceSize bytes);
    GpuBuffer uploadIndices (const void* data, vk::DeviceSize bytes);

    // Render commands recorded into every command buffer on rerecord().
    // Push lambdas that bind pipeline, vertex buffers, draw, etc.
    std::vector<std::function<void(vk::CommandBuffer)>> renderCmds;

    vk::ClearValue clearValue;

    // Compile SPIR-V bytes into a ShaderModule and store in shaders[].
    void compileShader(const uint8_t* spv, uint32_t len, vk::ShaderStageFlagBits stage);

    // Build a general pipeline with explicit vertex input layout.
    // pushConstBytes > 0 adds a vertex-stage push constant range.
    vk::Pipeline buildPipeline(
        const std::vector<vk::VertexInputBindingDescription>&   bindings,
        const std::vector<vk::VertexInputAttributeDescription>& attrs,
        uint32_t pushConstBytes = 0);

    // Convenience: no vertex input (positions hardcoded in shader).
    vk::Pipeline buildFlatPipeline(uint32_t pushConstBytes = 0);

    // Re-record all command buffers: clear → renderCmds → end.
    // Used for static scenes; not needed when onRecord is set.
    void rerecord();

    // Called each frame before recording — update animation state, UBOs, etc.
    std::function<void()> onFrame;

    // When set, called each frame after image acquire to dynamically record
    // draw commands into the command buffer for that swapchain image.
    // The render pass is already begun; just bind pipeline, push constants, draw.
    // Takes priority over the pre-recorded renderCmds list.
    std::function<void(vk::CommandBuffer, uint32_t imgIdx)> onRecord;

    // Run the GLFW event + draw loop until the window is closed.
    void wait();

    // ── Lifetime ─────────────────────────────────────────────────────────────
    VulkanContext() = default;
    ~VulkanContext();
    VulkanContext(const VulkanContext&)            = delete;
    VulkanContext& operator=(const VulkanContext&) = delete;
    VulkanContext(VulkanContext&& o) noexcept            { doSwap(o); }
    VulkanContext& operator=(VulkanContext&& o) noexcept { doSwap(o); return *this; }

private:
    GpuBuffer uploadBuffer(const void* data, vk::DeviceSize bytes,
                           vk::BufferUsageFlags usage);
    void doSwap(VulkanContext& o) noexcept;
};

// Creates a flat (color-only, no depth) Vulkan context.
// GLFW window is created internally; call ctx.wait() to run the loop.
VulkanContext createVulkanFlat(const char title[], uint32_t w, uint32_t h,
    const color& clearColor = {0.05f, 0.05f, 0.15f, 1.0f});

// Acquire next swapchain image, submit pre-recorded cmd buffer, present.
void drawFrame(VulkanContext& ctx);
