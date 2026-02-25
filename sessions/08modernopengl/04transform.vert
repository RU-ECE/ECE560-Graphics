#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

uniform mat4 transform;
out vec3 rgb;

void main(){
    gl_Position = transform*vec4(pos, 1.0);
    rgb = color;
}

