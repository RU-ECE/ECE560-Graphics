#pragma once

const char* vs_texture = R"(
#version 330 core

layout(location = 0) in vec3 pos;       // Position (x, y, z)
layout(location = 1) in vec2 texCoord;  // Texture coordinates (u, v)

out vec2 TexCoord;

void main() {
    gl_Position = vec4(pos, 1.0);
    TexCoord = texCoord;
}
)";

const char* fs_texture = R"(
#version 330 core

in vec2 TexCoord;
out vec4 color;

uniform sampler2D ourTexture;

void main() {
    color = texture(ourTexture, TexCoord);
}
)";
