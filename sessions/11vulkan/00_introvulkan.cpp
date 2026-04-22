#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>


static uint32_t findQueueFamily(vk::PhysicalDevice pd, vk::SurfaceKHR surface) {
    auto props = pd.getQueueFamilyProperties();
    for (uint32_t i = 0; i < (uint32_t)props.size(); i++) {
        if ((props[i].queueFlags & vk::QueueFlagBits::eGraphics) &&
            pd.getSurfaceSupportKHR(i, surface))
            return i;
    }
    throw std::runtime_error("no graphics+present queue family");
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    const uint32_t W = 800, H = 600;
    GLFWwindow* window = glfwCreateWindow(W, H, "Vulkan", nullptr, nullptr);

    // Instance with surface extensions required by GLFW
    uint32_t extCount = 0;
    const char** glfwExts = glfwGetRequiredInstanceExtensions(&extCount);
    std::vector<const char*> exts(glfwExts, glfwExts + extCount);

    auto appInfo = vk::ApplicationInfo{}
        .setPApplicationName("BlankWindow")
        .setApiVersion(VK_API_VERSION_1_0);

    vk::Instance instance = vk::createInstance(
        vk::InstanceCreateInfo{}
            .setPApplicationInfo(&appInfo)
            .setPEnabledExtensionNames(exts));

    // Surface (GLFW gives us VkSurfaceKHR; wrap it)
    VkSurfaceKHR rawSurface;
    if (glfwCreateWindowSurface(instance, window, nullptr, &rawSurface) != VK_SUCCESS)
        throw std::runtime_error("failed to create surface");
    vk::SurfaceKHR surface(rawSurface);

    // Pick first physical device with a graphics+present queue
    vk::PhysicalDevice physDev;
    uint32_t qFamilyIdx = 0;
    for (auto& pd : instance.enumeratePhysicalDevices()) {
        try {
            qFamilyIdx = findQueueFamily(pd, surface);
            physDev = pd;
            break;
        } catch (...) {}
    }
    if (!physDev) throw std::runtime_error("no suitable GPU");

    // Logical device
    float qPriority = 1.0f;
    auto queueInfo = vk::DeviceQueueCreateInfo{}
        .setQueueFamilyIndex(qFamilyIdx)
        .setQueuePriorities(qPriority);

    const char* swapExt = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    vk::Device device = physDev.createDevice(
        vk::DeviceCreateInfo{}
            .setQueueCreateInfos(queueInfo)
            .setPEnabledExtensionNames(swapExt));

    vk::Queue queue = device.getQueue(qFamilyIdx, 0);

    // Swapchain
    auto caps    = physDev.getSurfaceCapabilitiesKHR(surface);
    auto formats = physDev.getSurfaceFormatsKHR(surface);

    vk::SurfaceFormatKHR fmt = formats[0];
    for (auto& f : formats)
        if (f.format == vk::Format::eB8G8R8A8Srgb &&
            f.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
            { fmt = f; break; }

    vk::Extent2D extent = (caps.currentExtent.width != UINT32_MAX)
        ? caps.currentExtent : vk::Extent2D{W, H};

    vk::SwapchainKHR swapchain = device.createSwapchainKHR(
        vk::SwapchainCreateInfoKHR{}
            .setSurface(surface)
            .setMinImageCount(caps.minImageCount + 1)
            .setImageFormat(fmt.format)
            .setImageColorSpace(fmt.colorSpace)
            .setImageExtent(extent)
            .setImageArrayLayers(1)
            .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
            .setImageSharingMode(vk::SharingMode::eExclusive)
            .setPreTransform(caps.currentTransform)
            .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
            .setPresentMode(vk::PresentModeKHR::eFifo)
            .setClipped(VK_TRUE));

    auto swapImages = device.getSwapchainImagesKHR(swapchain);

    // Image views
    std::vector<vk::ImageView> views;
    for (auto& img : swapImages)
        views.push_back(device.createImageView(
            vk::ImageViewCreateInfo{}
                .setImage(img)
                .setViewType(vk::ImageViewType::e2D)
                .setFormat(fmt.format)
                .setSubresourceRange(
                    vk::ImageSubresourceRange{}
                        .setAspectMask(vk::ImageAspectFlagBits::eColor)
                        .setLevelCount(1)
                        .setLayerCount(1))));

    // Render pass: single color attachment, clear on load, present layout on store
    auto attachment = vk::AttachmentDescription{}
        .setFormat(fmt.format)
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

    // Dependency to synchronise swapchain image layout transition
    auto dep = vk::SubpassDependency{}
        .setSrcSubpass(VK_SUBPASS_EXTERNAL)
        .setDstSubpass(0)
        .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
        .setSrcAccessMask(vk::AccessFlags{})
        .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
        .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);

    vk::RenderPass renderPass = device.createRenderPass(
        vk::RenderPassCreateInfo{}
            .setAttachments(attachment)
            .setSubpasses(subpass)
            .setDependencies(dep));

    // Framebuffers (one per swapchain image)
    std::vector<vk::Framebuffer> framebuffers;
    for (auto& v : views)
        framebuffers.push_back(device.createFramebuffer(
            vk::FramebufferCreateInfo{}
                .setRenderPass(renderPass)
                .setAttachments(v)
                .setWidth(extent.width)
                .setHeight(extent.height)
                .setLayers(1)));

    // Command pool + one command buffer per swapchain image
    vk::CommandPool cmdPool = device.createCommandPool(
        vk::CommandPoolCreateInfo{}
            .setQueueFamilyIndex(qFamilyIdx));

    auto cmds = device.allocateCommandBuffers(
        vk::CommandBufferAllocateInfo{}
            .setCommandPool(cmdPool)
            .setLevel(vk::CommandBufferLevel::ePrimary)
            .setCommandBufferCount((uint32_t)swapImages.size()));

    // Pre-record: begin render pass (clears to dark blue), end — no draw calls needed
    vk::ClearValue clearColor(vk::ClearColorValue{std::array<float,4>{0.05f, 0.05f, 0.15f, 1.0f}});
    for (size_t i = 0; i < cmds.size(); i++) {
        cmds[i].begin(vk::CommandBufferBeginInfo{});
        cmds[i].beginRenderPass(
            vk::RenderPassBeginInfo{}
                .setRenderPass(renderPass)
                .setFramebuffer(framebuffers[i])
                .setRenderArea(vk::Rect2D{{0, 0}, extent})
                .setClearValues(clearColor),
            vk::SubpassContents::eInline);
        cmds[i].endRenderPass();
        cmds[i].end();
    }

    // Sync: 2 frames in flight
    const uint32_t MAX_FRAMES = 2;
    std::vector<vk::Semaphore> imgAvail(MAX_FRAMES), renderDone(MAX_FRAMES);
    std::vector<vk::Fence>     inFlight(MAX_FRAMES);
    std::vector<vk::Fence>     imgFences(swapImages.size(), nullptr);

    for (uint32_t i = 0; i < MAX_FRAMES; i++) {
        imgAvail[i]   = device.createSemaphore({});
        renderDone[i] = device.createSemaphore({});
        inFlight[i]   = device.createFence(
            vk::FenceCreateInfo{}.setFlags(vk::FenceCreateFlagBits::eSignaled));
    }

    // Render loop
    uint32_t frame = 0;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        (void)device.waitForFences(inFlight[frame], VK_TRUE, UINT64_MAX);

        uint32_t imgIdx;
        (void)device.acquireNextImageKHR(
            swapchain, UINT64_MAX, imgAvail[frame], nullptr, &imgIdx);

        if (imgFences[imgIdx])
            (void)device.waitForFences(imgFences[imgIdx], VK_TRUE, UINT64_MAX);
        imgFences[imgIdx] = inFlight[frame];

        device.resetFences(inFlight[frame]);

        vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        queue.submit(
            vk::SubmitInfo{}
                .setWaitSemaphores(imgAvail[frame])
                .setWaitDstStageMask(waitStage)
                .setCommandBuffers(cmds[imgIdx])
                .setSignalSemaphores(renderDone[frame]),
            inFlight[frame]);

        (void)queue.presentKHR(
            vk::PresentInfoKHR{}
                .setWaitSemaphores(renderDone[frame])
                .setSwapchains(swapchain)
                .setImageIndices(imgIdx));

        frame = (frame + 1) % MAX_FRAMES;
    }

    device.waitIdle();

    for (uint32_t i = 0; i < MAX_FRAMES; i++) {
        device.destroySemaphore(imgAvail[i]);
        device.destroySemaphore(renderDone[i]);
        device.destroyFence(inFlight[i]);
    }
    device.freeCommandBuffers(cmdPool, cmds);
    device.destroyCommandPool(cmdPool);
    for (auto& fb : framebuffers) device.destroyFramebuffer(fb);
    device.destroyRenderPass(renderPass);
    for (auto& v : views) device.destroyImageView(v);
    device.destroySwapchainKHR(swapchain);
    device.destroy();
    instance.destroySurfaceKHR(surface);
    instance.destroy();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
