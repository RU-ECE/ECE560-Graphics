#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <array>

using namespace std;

// --- Data Structures ---
struct Glyph {
    glm::vec2 uv0, uv1; // texture coords
    glm::vec2 size;     // width/height
    glm::vec2 offset;   // baseline offset
};

struct Vertex {
    glm::vec2 pos;
    glm::vec2 uv;
};

struct VkDrawIndirectCommand {
    uint32_t vertexCount;
    uint32_t instanceCount;
    uint32_t firstVertex;
    uint32_t firstInstance;
};

// --- Buffers ---
vk::Buffer charBuffer;      // input array of character codes
vk::Buffer glyphBuffer;     // per-glyph info
vk::Buffer vertexBuffer;    // output vertex buffer
vk::Buffer indirectBuffer;  // indirect draw command
uint32_t numChars;

// --- GLSL Compute Shader ---
const char* computeShaderGLSL = R"(
#version 450
layout(local_size_x = 64) in;

struct Glyph {
    vec2 uv0, uv1;
    vec2 size;
    vec2 offset;
};

layout(std430, binding = 0) readonly buffer CharBuffer { uint chars[]; };
layout(std430, binding = 1) readonly buffer GlyphBuffer { Glyph glyphs[]; };
layout(std430, binding = 2) writeonly buffer VertexBuffer { vec2 positions[]; vec2 uvs[]; };
layout(std430, binding = 3) writeonly buffer IndirectBuffer { uint vertexCount; uint instanceCount; uint firstVertex; uint firstInstance; } drawCommand;

void main() {
    uint idx = gl_GlobalInvocationID.x;
    if (idx >= chars.length()) return;

    uint c = chars[idx];
    Glyph g = glyphs[c];

    vec2 basePos = vec2(idx*0.05, 0.0) + g.offset;

    // Sequential quad vertices (triangle strip: bottom-left, bottom-right, top-left, top-right)
    vec2 pos[4] = vec2[4](basePos,
                          basePos + vec2(g.size.x,0),
                          basePos + vec2(0,g.size.y),
                          basePos + g.size);
    vec2 uv[4] = vec2[4](g.uv0, vec2(g.uv1.x,g.uv0.y),
                         vec2(g.uv0.x,g.uv1.y), g.uv1);

    uint outBase = idx*4;
    for(int i=0;i<4;i++){
        positions[outBase+i] = pos[i];
        uvs[outBase+i] = uv[i];
    }

    // First thread writes indirect draw command
    if(idx == 0){
        drawCommand.vertexCount = chars.length()*4;
        drawCommand.instanceCount = 1;
        drawCommand.firstVertex = 0;
        drawCommand.firstInstance = 0;
    }
}
)";

// --- Command Buffer Recording ---
void recordCommands(vk::CommandBuffer cmd, vk::Pipeline computePipeline, vk::Pipeline graphicsPipeline,
                    vk::PipelineLayout computeLayout, vk::PipelineLayout graphicsLayout,
                    vk::DescriptorSet computeDesc, vk::DescriptorSet graphicsDesc) {

    // Dispatch compute shader to generate vertices
    cmd.bindPipeline(vk::PipelineBindPoint::eCompute, computePipeline);
    cmd.bindDescriptorSets(vk::PipelineBindPoint::eCompute, computeLayout, 0, {computeDesc}, {});
    cmd.dispatch((numChars + 63)/64, 1, 1);

    // Memory barrier so vertex shader sees results
    vk::MemoryBarrier barrier(vk::AccessFlagBits::eShaderWrite,
                              vk::AccessFlagBits::eVertexAttributeRead | vk::AccessFlagBits::eIndirectCommandRead);
    cmd.pipelineBarrier(vk::PipelineStageFlagBits::eComputeShader,
                        vk::PipelineStageFlagBits::eDrawIndirect | vk::PipelineStageFlagBits::eVertexInput,
                        {}, barrier, {}, {});

    // Graphics pass: draw using generated vertex buffer
    cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);
    cmd.bindVertexBuffers(0, {vertexBuffer}, {0});
    cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, graphicsLayout, 0, {graphicsDesc}, {});
    cmd.drawIndirect(indirectBuffer, 0, 1, sizeof(VkDrawIndirectCommand));
}