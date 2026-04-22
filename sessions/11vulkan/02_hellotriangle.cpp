#include "vulkan_common.hpp"
#include "triangle_vert_spv.h"
#include "simple_frag_spv.h"

int main() {
    VulkanContext ctx = createVulkanFlat("Hello Triangle", 800, 600, {0.05f, 0.05f, 0.15f, 1.0f});

    ctx.compileShader(triangle_vert_spv, triangle_vert_spv_len, vk::ShaderStageFlagBits::eVertex);
    ctx.compileShader(simple_frag_spv,   simple_frag_spv_len,   vk::ShaderStageFlagBits::eFragment);

    vk::Pipeline pipe = ctx.buildFlatPipeline();

    ctx.renderCmds.push_back([pipe](vk::CommandBuffer cmd) {
        cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, pipe);
        cmd.draw(3, 1, 0, 0);
    });
    ctx.rerecord();

    ctx.wait();
    return 0;
}
