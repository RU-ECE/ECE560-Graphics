#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 v;
uniform mat4 trans;

void main(){
    gl_PointSize = 5;
    //gl_PointSize = gl_Normal.x;
    gl_Position = trans * vec4(v,1.0);
    gl_Position.w = 1.0;
}

