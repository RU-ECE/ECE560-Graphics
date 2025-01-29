#version 330 core

layout(location = 0) in vec3 pos;       // Position (x, y, z)
layout(location = 1) in vec2 texCoord;  // Texture coordinates (u, v)

uniform mat4 transform;

out vec2 TexCoord;

void main() {
    gl_Position = transform * vec4(pos, 1.0);
    TexCoord = texCoord;
}
