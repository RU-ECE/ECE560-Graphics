#pragma once

const char* const vs_gouraud = R"GLSL(
#version 330 core
layout ( location = 0 ) in vec2 pos;
layout ( location = 1 ) in vec3 Color;

out VertexData
{
    vec3 Color;
} vsOutput;
void main()
{
    gl_Position = vec4(pos,0.0,1.0);
    vsOutput.Color = Color;
}
)GLSL";

// this fragment shader can be used for any vertex shader that uses rgb colors stored per vertex
const char* const fs_common = R"GLSL(
#version 330 core
in VertexData
{
    vec3 Color;
} fsInput;
out vec4 outColor;
void main()
{
    outColor = vec4( fsInput.Color, 1.0 );
}
)GLSL";
