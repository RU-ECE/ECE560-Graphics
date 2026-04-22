#include "vulkan_common.hpp"
#include "sphere_with_indices.hpp"
#include "sphere_vert_spv.h"
#include "simple_frag_spv.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main() {
    VulkanContext ctx = createVulkanFlat("Spinning Sphere", 800, 600);

    SphereGenerator gen(48, 24, 0.8f);
    std::vector<VertexXYZRGB> verts;
    std::vector<uint32_t>     idx;
    gen.generateXYZRGB(verts, idx);

    auto vbuf = ctx.uploadVertices(verts.data(), verts.size() * sizeof(VertexXYZRGB));
    auto ibuf = ctx.uploadIndices (idx.data(),   idx.size()   * sizeof(uint32_t));

    ctx.compileShader(sphere_vert_spv, sphere_vert_spv_len, vk::ShaderStageFlagBits::eVertex);
    ctx.compileShader(simple_frag_spv, simple_frag_spv_len, vk::ShaderStageFlagBits::eFragment);

    std::vector<vk::VertexInputBindingDescription> bindings{
        {0, sizeof(VertexXYZRGB), vk::VertexInputRate::eVertex}
    };
    std::vector<vk::VertexInputAttributeDescription> attrs{
        {0, 0, vk::Format::eR32G32B32Sfloat, 0},                          // pos
        {1, 0, vk::Format::eR32G32B32Sfloat, offsetof(VertexXYZRGB, r)}   // color
    };
    vk::Pipeline       pipe   = ctx.buildPipeline(bindings, attrs, sizeof(glm::mat4));
    vk::PipelineLayout layout = ctx.pipelineLayouts.back();

    const uint32_t idxCount = (uint32_t)idx.size();
    glm::mat4 mvp(1.0f);
    float angle = 0.0f;

    ctx.onFrame = [&]() {
        angle += 0.005f;
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 view  = glm::lookAt(glm::vec3(0.0f, 0.5f, 2.5f),
                                      glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 proj  = glm::perspective(glm::radians(45.0f),
                                           800.0f / 600.0f, 0.1f, 10.0f);
        proj[1][1] *= -1.0f;  // Vulkan NDC has Y pointing down
        mvp = proj * view * model;
    };

    ctx.onRecord = [&](vk::CommandBuffer cmd, uint32_t) {
        cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, pipe);
        cmd.bindVertexBuffers(0, vbuf.buffer, vk::DeviceSize{0});
        cmd.bindIndexBuffer(ibuf.buffer, 0, vk::IndexType::eUint32);
        cmd.pushConstants(layout, vk::ShaderStageFlagBits::eVertex,
                          0, sizeof(glm::mat4), &mvp);
        cmd.drawIndexed(idxCount, 1, 0, 0, 0);
    };

    ctx.wait();
    return 0;
}
