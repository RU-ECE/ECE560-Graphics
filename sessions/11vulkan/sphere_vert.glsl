// sphere_vert.glsl
#version 450

layout(push_constant) uniform PC { mat4 mvp; } pc;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = pc.mvp * vec4(pos, 1.0);
    fragColor   = color;
}
