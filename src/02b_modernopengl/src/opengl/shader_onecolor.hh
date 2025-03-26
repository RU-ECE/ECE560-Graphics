#pragma once

const char* const vs_onecolor = R"GLSL(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec3 color;

out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = color;
}
)GLSL";

const char* const fs_onecolor = R"GLSL(
#version 330 core
in vec3 ourColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}
)GLSL";