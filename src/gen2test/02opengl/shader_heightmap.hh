#pragma once

/*
 
 this shader implements a height map using separate inputs for the x,y and z coordinates
 THe x,y will typically not change, while the z will change as we move around the larger
 virtual world.

 This first version will work without normals, just the height map 
*/

const char* vs_height_map = R"(
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;    

out vec3 frag_pos;
//out vec3 frag_normal;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(pos, 1.0);
}
)";


// the fragment shader will just color the terrain in grayscale from dark to light
// based on the height of the terrain
// 0.2 is the gray scale for the minimum height and 1.0 is the gray scale for the maximum height
const char* fs_height_map = R"(
#version 330 core

layout (location = 0) in vec2 xy;
layout (location = 1) in float z;

in vec3 frag_pos;
in vec3 frag_normal;

out vec4 color;

uniform float min_height;
uniform float max_height;

void main() {
    float height = frag_pos.z;
    float gray_scale = (height - min_height) / (max_height - min_height);
    color = vec4(gray_scale, gray_scale, gray_scale, 1.0);
}
)";
