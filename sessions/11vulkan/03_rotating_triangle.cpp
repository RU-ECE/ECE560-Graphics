#include "vulkan_common.hpp"
#include "triangle_transform_vert_spv.h"
#include "simple_frag_spv.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main() {
    VulkanContext ctx = createVulkanFlat("Rotating Triangle", 800, 600);

    ctx.compileShader(triangle_transform_vert_spv, triangle_transform_vert_spv_len,
                      vk::ShaderStageFlagBits::eVertex);
    ctx.compileShader(simple_frag_spv, simple_frag_spv_len,
                      vk::ShaderStageFlagBits::eFragment);

    vk::Pipeline       pipe   = ctx.buildFlatPipeline(sizeof(glm::mat4));
    vk::PipelineLayout layout = ctx.pipelineLayouts.back();

    glm::mat4 mvp(1.0f);
    float     angle = 0.0f;

    ctx.onFrame = [&]() {
        angle += 0.01f;
        mvp = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
    };

    ctx.onRecord = [&](vk::CommandBuffer cmd, uint32_t) {
        cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, pipe);
        cmd.pushConstants(layout, vk::ShaderStageFlagBits::eVertex,
                          0, sizeof(glm::mat4), &mvp);
        cmd.draw(3, 1, 0, 0);
    };

    ctx.wait();
    return 0;
}
