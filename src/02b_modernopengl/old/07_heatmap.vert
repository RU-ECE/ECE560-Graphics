#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 v;

uniform mat4 trans;
out float val;
void main(){
    gl_Position = trans * vec4(v,1.0);
    val = v.z;
}

