#version 460 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

out vec3 rgb;
void main(){
    gl_Position = vec4(pos, 1.0);
    rgb = color;
}

