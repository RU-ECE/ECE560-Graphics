#include "vulkan_common.hpp"
#include <stdexcept>

// ── VulkanContext lifetime ────────────────────────────────────────────────────

VulkanContext::~VulkanContext() {
    if (!device) return;
    device.waitIdle();
    for (auto& s : shaders)         device.destroyShaderModule(s.module);
    for (auto& p : pipelines)       device.destroyPipeline(p);
    for (auto& l : pipelineLayouts) device.destroyPipelineLayout(l);
    for (auto& b : buffers) { device.destroyBuffer(b.buffer); device.freeMemory(b.memory); }
    for (uint32_t i = 0; i < MAX_FRAMES; i++) {
        device.destroySemaphore(imgAvail[i]);
        device.destroySemaphore(renderDone[i]);
        device.destroyFence(inFlight[i]);
    }
    device.freeCommandBuffers(cmdPool, cmds);
    device.destroyCommandPool(cmdPool);
    for (auto& fb : framebuffers) device.destroyFramebuffer(fb);
    device.destroyRenderPass(renderPass);
    for (auto& v : imageViews) device.destroyImageView(v);
    device.destroySwapchainKHR(swapchain);
    device.destroy();
    instance.destroySurfaceKHR(surface);
    instance.destroy();
    glfwDestroyWindow(win);
    glfwTerminate();
}

void VulkanContext::doSwap(VulkanContext& o) noexcept {
    using std::swap;
    swap(win,        o.win);
    swap(instance,   o.instance);
    swap(surface,    o.surface);
    swap(physDev,    o.physDev);
    swap(device,     o.device);
    swap(queue,      o.queue);
    swap(qFamilyIdx, o.qFamilyIdx);
    swap(surfaceFmt, o.surfaceFmt);
    swap(extent,     o.extent);
    swap(swapchain,  o.swapchain);
    swap(swapImages, o.swapImages);
    swap(imageViews, o.imageViews);
    swap(renderPass, o.renderPass);
    swap(framebuffers, o.framebuffers);
    swap(cmdPool,    o.cmdPool);
    swap(cmds,       o.cmds);
    swap(imgAvail,   o.imgAvail);
    swap(renderDone, o.renderDone);
    swap(inFlight,   o.inFlight);
    swap(imgFences,      o.imgFences);
    swap(frame,          o.frame);
    swap(shaders,         o.shaders);
    swap(pipelines,       o.pipelines);
    swap(pipelineLayouts, o.pipelineLayouts);
    swap(buffers,         o.buffers);
    swap(renderCmds,      o.renderCmds);
    swap(onFrame,        o.onFrame);
    swap(onRecord,       o.onRecord);
    swap(clearValue,     o.clearValue);
}

// ── Helpers ───────────────────────────────────────────────────────────────────

static uint32_t findQueueFamily(vk::PhysicalDevice pd, vk::SurfaceKHR surface) {
    auto props = pd.getQueueFamilyProperties();
    for (uint32_t i = 0; i < (uint32_t)props.size(); i++) {
        if ((props[i].queueFlags & vk::QueueFlagBits::eGraphics) &&
            pd.getSurfaceSupportKHR(i, surface))
            return i;
    }
    throw std::runtime_error("no graphics+present queue family");
}

void VulkanContext::wait() {
    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        drawFrame(*this);
    }
}

// ── Factory ───────────────────────────────────────────────────────────────────

VulkanContext createVulkanFlat(const char title[], uint32_t w, uint32_t h,
                               const color& clearColor) {
    VulkanContext ctx;
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    ctx.win = glfwCreateWindow(w, h, title, nullptr, nullptr);
                            
 
    // Instance
    uint32_t extCount = 0;
    const char** glfwExts = glfwGetRequiredInstanceExtensions(&extCount);
    std::vector<const char*> exts(glfwExts, glfwExts + extCount);
    auto appInfo = vk::ApplicationInfo{}.setApiVersion(VK_API_VERSION_1_0);
    ctx.instance = vk::createInstance(
        vk::InstanceCreateInfo{}
            .setPApplicationInfo(&appInfo)
            .setPEnabledExtensionNames(exts));

    // Surface
    VkSurfaceKHR raw;
    if (glfwCreateWindowSurface(ctx.instance, ctx.win, nullptr, &raw) != VK_SUCCESS)
        throw std::runtime_error("failed to create surface");
    ctx.surface = vk::SurfaceKHR(raw);

    // Physical device — first one with a graphics+present queue
    for (auto& pd : ctx.instance.enumeratePhysicalDevices()) {
        try {
            ctx.qFamilyIdx = findQueueFamily(pd, ctx.surface);
            ctx.physDev = pd;
            break;
        } catch (...) {}
    }
    if (!ctx.physDev) throw std::runtime_error("no suitable GPU");

    // Logical device + queue
    float qPriority = 1.0f;
    auto queueInfo = vk::DeviceQueueCreateInfo{}
        .setQueueFamilyIndex(ctx.qFamilyIdx)
        .setQueuePriorities(qPriority);
    const char* swapExt = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    ctx.device = ctx.physDev.createDevice(
        vk::DeviceCreateInfo{}
            .setQueueCreateInfos(queueInfo)
            .setPEnabledExtensionNames(swapExt));
    ctx.queue = ctx.device.getQueue(ctx.qFamilyIdx, 0);

    // Swapchain
    auto caps    = ctx.physDev.getSurfaceCapabilitiesKHR(ctx.surface);
    auto formats = ctx.physDev.getSurfaceFormatsKHR(ctx.surface);
    ctx.surfaceFmt = formats[0];
    for (auto& f : formats)
        if (f.format == vk::Format::eB8G8R8A8Srgb &&
            f.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
            { ctx.surfaceFmt = f; break; }
    ctx.extent = (caps.currentExtent.width != UINT32_MAX)
        ? caps.currentExtent : vk::Extent2D{w, h};
    ctx.swapchain = ctx.device.createSwapchainKHR(
        vk::SwapchainCreateInfoKHR{}
            .setSurface(ctx.surface)
            .setMinImageCount(caps.minImageCount + 1)
            .setImageFormat(ctx.surfaceFmt.format)
            .setImageColorSpace(ctx.surfaceFmt.colorSpace)
            .setImageExtent(ctx.extent)
            .setImageArrayLayers(1)
            .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
            .setImageSharingMode(vk::SharingMode::eExclusive)
            .setPreTransform(caps.currentTransform)
            .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
            .setPresentMode(vk::PresentModeKHR::eFifo)
            .setClipped(VK_TRUE));
    ctx.swapImages = ctx.device.getSwapchainImagesKHR(ctx.swapchain);

    // Image views
    for (auto& img : ctx.swapImages)
        ctx.imageViews.push_back(ctx.device.createImageView(
            vk::ImageViewCreateInfo{}
                .setImage(img)
                .setViewType(vk::ImageViewType::e2D)
                .setFormat(ctx.surfaceFmt.format)
                .setSubresourceRange(
                    vk::ImageSubresourceRange{}
                        .setAspectMask(vk::ImageAspectFlagBits::eColor)
                        .setLevelCount(1)
                        .setLayerCount(1))));

    // Render pass: color attachment only, clear on load, transition to present layout
    auto attachment = vk::AttachmentDescription{}
        .setFormat(ctx.surfaceFmt.format)
        .setSamples(vk::SampleCountFlagBits::e1)
        .setLoadOp(vk::AttachmentLoadOp::eClear)
        .setStoreOp(vk::AttachmentStoreOp::eStore)
        .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
        .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
        .setInitialLayout(vk::ImageLayout::eUndefined)
        .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);
    auto colorRef = vk::AttachmentReference{}
        .setAttachment(0)
        .setLayout(vk::ImageLayout::eColorAttachmentOptimal);
    auto subpass = vk::SubpassDescription{}
        .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
        .setColorAttachments(colorRef);
    auto dep = vk::SubpassDependency{}
        .setSrcSubpass(VK_SUBPASS_EXTERNAL).setDstSubpass(0)
        .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
        .setSrcAccessMask(vk::AccessFlags{})
        .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
        .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);
    ctx.renderPass = ctx.device.createRenderPass(
        vk::RenderPassCreateInfo{}
            .setAttachments(attachment)
            .setSubpasses(subpass)
            .setDependencies(dep));

    // Framebuffers
    for (auto& v : ctx.imageViews)
        ctx.framebuffers.push_back(ctx.device.createFramebuffer(
            vk::FramebufferCreateInfo{}
                .setRenderPass(ctx.renderPass)
                .setAttachments(v)
                .setWidth(ctx.extent.width)
                .setHeight(ctx.extent.height)
                .setLayers(1)));

    // Command pool — eResetCommandBuffer allows per-buffer reset for dynamic recording
    ctx.cmdPool = ctx.device.createCommandPool(
        vk::CommandPoolCreateInfo{}
            .setQueueFamilyIndex(ctx.qFamilyIdx)
            .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer));
    ctx.clearValue = vk::ClearValue(
        vk::ClearColorValue{clearColor.r, clearColor.g, clearColor.b, clearColor.a});

    // Sync objects
    ctx.imgAvail.resize(VulkanContext::MAX_FRAMES);
    ctx.renderDone.resize(VulkanContext::MAX_FRAMES);
    ctx.inFlight.resize(VulkanContext::MAX_FRAMES);
    ctx.imgFences.resize(ctx.swapImages.size(), nullptr);
    for (uint32_t i = 0; i < VulkanContext::MAX_FRAMES; i++) {
        ctx.imgAvail[i]   = ctx.device.createSemaphore({});
        ctx.renderDone[i] = ctx.device.createSemaphore({});
        ctx.inFlight[i]   = ctx.device.createFence(
            vk::FenceCreateInfo{}.setFlags(vk::FenceCreateFlagBits::eSignaled));
    }

    ctx.rerecord();
    return ctx;
}

// ── Memory / buffer helpers ───────────────────────────────────────────────────

static uint32_t findMemoryType(vk::PhysicalDevice pd, uint32_t filter,
                                vk::MemoryPropertyFlags props) {
    auto mem = pd.getMemoryProperties();
    for (uint32_t i = 0; i < mem.memoryTypeCount; i++)
        if ((filter & (1u << i)) && (mem.memoryTypes[i].propertyFlags & props) == props)
            return i;
    throw std::runtime_error("no suitable memory type");
}

VulkanContext::GpuBuffer VulkanContext::uploadBuffer(const void* data,
                                                     vk::DeviceSize bytes,
                                                     vk::BufferUsageFlags usage) {
    // Staging buffer: host-visible + coherent
    auto stageBuf = device.createBuffer(
        vk::BufferCreateInfo{}.setSize(bytes)
            .setUsage(vk::BufferUsageFlagBits::eTransferSrc));
    auto stageReq = device.getBufferMemoryRequirements(stageBuf);
    auto stageMem = device.allocateMemory(vk::MemoryAllocateInfo{}
        .setAllocationSize(stageReq.size)
        .setMemoryTypeIndex(findMemoryType(physDev, stageReq.memoryTypeBits,
            vk::MemoryPropertyFlagBits::eHostVisible |
            vk::MemoryPropertyFlagBits::eHostCoherent)));
    device.bindBufferMemory(stageBuf, stageMem, 0);
    void* mapped = device.mapMemory(stageMem, 0, bytes);
    memcpy(mapped, data, (size_t)bytes);
    device.unmapMemory(stageMem);

    // Device-local buffer
    auto devBuf = device.createBuffer(
        vk::BufferCreateInfo{}.setSize(bytes).setUsage(usage));
    auto devReq = device.getBufferMemoryRequirements(devBuf);
    auto devMem = device.allocateMemory(vk::MemoryAllocateInfo{}
        .setAllocationSize(devReq.size)
        .setMemoryTypeIndex(findMemoryType(physDev, devReq.memoryTypeBits,
            vk::MemoryPropertyFlagBits::eDeviceLocal)));
    device.bindBufferMemory(devBuf, devMem, 0);

    // One-shot copy
    auto cmd = device.allocateCommandBuffers(vk::CommandBufferAllocateInfo{}
        .setCommandPool(cmdPool).setLevel(vk::CommandBufferLevel::ePrimary)
        .setCommandBufferCount(1))[0];
    cmd.begin(vk::CommandBufferBeginInfo{}
        .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));
    cmd.copyBuffer(stageBuf, devBuf, vk::BufferCopy{}.setSize(bytes));
    cmd.end();

    auto fence = device.createFence({});
    queue.submit(vk::SubmitInfo{}.setCommandBuffers(cmd), fence);
    (void)device.waitForFences(fence, VK_TRUE, UINT64_MAX);
    device.destroyFence(fence);
    device.freeCommandBuffers(cmdPool, cmd);
    device.destroyBuffer(stageBuf);
    device.freeMemory(stageMem);

    GpuBuffer result{devBuf, devMem};
    buffers.push_back(result);
    return result;
}

VulkanContext::GpuBuffer VulkanContext::uploadVertices(const void* data, vk::DeviceSize bytes) {
    return uploadBuffer(data, bytes,
        vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst);
}

VulkanContext::GpuBuffer VulkanContext::uploadIndices(const void* data, vk::DeviceSize bytes) {
    return uploadBuffer(data, bytes,
        vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst);
}

// ── Shader / pipeline helpers ─────────────────────────────────────────────────

void VulkanContext::compileShader(const uint8_t* spv, uint32_t len,
                                  vk::ShaderStageFlagBits stage) {
    auto mod = device.createShaderModule(
        vk::ShaderModuleCreateInfo{}
            .setCodeSize(len)
            .setPCode(reinterpret_cast<const uint32_t*>(spv)));
    shaders.push_back({mod, stage});
}

vk::Pipeline VulkanContext::buildPipeline(
        const std::vector<vk::VertexInputBindingDescription>&   bindings,
        const std::vector<vk::VertexInputAttributeDescription>& attrs,
        uint32_t pushConstBytes) {

    vk::PushConstantRange pcRange{vk::ShaderStageFlagBits::eVertex, 0, pushConstBytes};
    auto layoutInfo = vk::PipelineLayoutCreateInfo{};
    if (pushConstBytes > 0) layoutInfo.setPushConstantRanges(pcRange);
    auto layout = device.createPipelineLayout(layoutInfo);
    pipelineLayouts.push_back(layout);

    std::vector<vk::PipelineShaderStageCreateInfo> stages;
    for (auto& s : shaders)
        stages.push_back(vk::PipelineShaderStageCreateInfo{}
            .setStage(s.stage).setModule(s.module).setPName("main"));

    auto vertexInput = vk::PipelineVertexInputStateCreateInfo{}
        .setVertexBindingDescriptions(bindings)
        .setVertexAttributeDescriptions(attrs);
    auto inputAssembly = vk::PipelineInputAssemblyStateCreateInfo{}
        .setTopology(vk::PrimitiveTopology::eTriangleList);

    vk::Viewport vp{0, 0, (float)extent.width, (float)extent.height, 0, 1};
    vk::Rect2D   sc{{0, 0}, extent};
    auto viewportState = vk::PipelineViewportStateCreateInfo{}
        .setViewports(vp).setScissors(sc);

    auto rasterizer = vk::PipelineRasterizationStateCreateInfo{}
        .setPolygonMode(vk::PolygonMode::eFill)
        .setCullMode(vk::CullModeFlagBits::eBack)
        .setFrontFace(vk::FrontFace::eClockwise)
        .setLineWidth(1.0f);

    auto multisampling = vk::PipelineMultisampleStateCreateInfo{}
        .setRasterizationSamples(vk::SampleCountFlagBits::e1);

    auto blendAttach = vk::PipelineColorBlendAttachmentState{}
        .setColorWriteMask(
            vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
            vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
    auto colorBlend = vk::PipelineColorBlendStateCreateInfo{}
        .setAttachments(blendAttach);

    auto [result, pipeline] = device.createGraphicsPipeline(nullptr,
        vk::GraphicsPipelineCreateInfo{}
            .setStages(stages)
            .setPVertexInputState(&vertexInput)
            .setPInputAssemblyState(&inputAssembly)
            .setPViewportState(&viewportState)
            .setPRasterizationState(&rasterizer)
            .setPMultisampleState(&multisampling)
            .setPColorBlendState(&colorBlend)
            .setLayout(layout)
            .setRenderPass(renderPass));

    pipelines.push_back(pipeline);
    return pipeline;
}

vk::Pipeline VulkanContext::buildFlatPipeline(uint32_t pushConstBytes) {
    return buildPipeline({}, {}, pushConstBytes);
}

void VulkanContext::rerecord() {
    if (!cmds.empty())
        device.freeCommandBuffers(cmdPool, cmds);

    cmds = device.allocateCommandBuffers(
        vk::CommandBufferAllocateInfo{}
            .setCommandPool(cmdPool)
            .setLevel(vk::CommandBufferLevel::ePrimary)
            .setCommandBufferCount((uint32_t)swapImages.size()));

    for (size_t i = 0; i < cmds.size(); i++) {
        cmds[i].begin(vk::CommandBufferBeginInfo{});
        cmds[i].beginRenderPass(
            vk::RenderPassBeginInfo{}
                .setRenderPass(renderPass)
                .setFramebuffer(framebuffers[i])
                .setRenderArea(vk::Rect2D{{0, 0}, extent})
                .setClearValues(clearValue),
            vk::SubpassContents::eInline);
        for (auto& fn : renderCmds)
            fn(cmds[i]);
        cmds[i].endRenderPass();
        cmds[i].end();
    }
}

// ── Per-frame rendering ───────────────────────────────────────────────────────


void drawFrame(VulkanContext& ctx) {
    // Per-frame logic (update animation state, write UBOs, etc.) runs first
    // so values are ready before we record or submit.
    if (ctx.onFrame) ctx.onFrame();

    uint32_t f = ctx.frame;
    (void)ctx.device.waitForFences(ctx.inFlight[f], VK_TRUE, UINT64_MAX);

    uint32_t imgIdx;
    (void)ctx.device.acquireNextImageKHR(
        ctx.swapchain, UINT64_MAX, ctx.imgAvail[f], nullptr, &imgIdx);

    if (ctx.imgFences[imgIdx])
        (void)ctx.device.waitForFences(ctx.imgFences[imgIdx], VK_TRUE, UINT64_MAX);
    ctx.imgFences[imgIdx] = ctx.inFlight[f];

    // Dynamic recording: re-record this image's command buffer every frame.
    // Used when draw commands or push constants change per frame.
    if (ctx.onRecord) {
        auto& cmd = ctx.cmds[imgIdx];
        cmd.reset();
        cmd.begin(vk::CommandBufferBeginInfo{}
            .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));
        cmd.beginRenderPass(
            vk::RenderPassBeginInfo{}
                .setRenderPass(ctx.renderPass)
                .setFramebuffer(ctx.framebuffers[imgIdx])
                .setRenderArea(vk::Rect2D{{0, 0}, ctx.extent})
                .setClearValues(ctx.clearValue),
            vk::SubpassContents::eInline);
        ctx.onRecord(cmd, imgIdx);
        cmd.endRenderPass();
        cmd.end();
    }

    ctx.device.resetFences(ctx.inFlight[f]);

    vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    ctx.queue.submit(
        vk::SubmitInfo{}
            .setWaitSemaphores(ctx.imgAvail[f])
            .setWaitDstStageMask(waitStage)
            .setCommandBuffers(ctx.cmds[imgIdx])
            .setSignalSemaphores(ctx.renderDone[f]),
        ctx.inFlight[f]);

    (void)ctx.queue.presentKHR(
        vk::PresentInfoKHR{}
            .setWaitSemaphores(ctx.renderDone[f])
            .setSwapchains(ctx.swapchain)
            .setImageIndices(imgIdx));

    ctx.frame = (f + 1) % VulkanContext::MAX_FRAMES;
}
