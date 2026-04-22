// triangle_transform_vert.glsl
#version 450

layout(push_constant) uniform PC { mat4 mvp; } pc;

layout(location = 0) out vec3 color;

void main() {
    vec2 positions[3] = vec2[](
        vec2( 0.0, -0.5),
        vec2( 0.5,  0.5),
        vec2(-0.5,  0.5)
    );
    vec3 colors[3] = vec3[](
        vec3(1.0, 0.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(0.0, 0.0, 1.0)
    );
    gl_Position = pc.mvp * vec4(positions[gl_VertexIndex], 0.0, 1.0);
    color = colors[gl_VertexIndex];
}
