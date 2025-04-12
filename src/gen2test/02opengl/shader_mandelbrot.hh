#pragma once

const char* vs_mandelbrot = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

out vec2 fragCoord;

void main() {
    fragCoord = aPos.xy;  // pass position to fragment shader for computation
    gl_Position = vec4(aPos, 1.0);
}
)";

const char* fs_mandelbrot = R"(
#version 330 core
in vec2 fragCoord;
out vec4 FragColor;

uniform float xmin, xmax, ymin, ymax;  // mandelbrot view bounds
uniform int maxiter;                    // maximum iterations
uniform sampler1D colormap;            // color lookup texture

void main() {
    // Map normalized screen coordinates (-1 to 1) to complex plane
    float x0 = (fragCoord.x + 1.0) * 0.5 * (xmax - xmin) + xmin;
    float y0 = (fragCoord.y + 1.0) * 0.5 * (ymax - ymin) + ymin;
    
    float x = 0.0;
    float y = 0.0;
    int iter = 0;
    
    // z = z^2 + c
    while (x*x + y*y <= 4.0 && iter < maxiter) {
        float xtemp = x*x - y*y + x0;
        y = 2.0*x*y + y0;
        x = xtemp;
        iter++;
    }
    
    // Simple linear mapping of iterations to texture coordinate
    float v = float(iter) * (1.0/float(maxiter));
    FragColor = texture(colormap, 0);
}
)";

const char* fs_debug = R"(
#version 330 core
in vec2 fragCoord;
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);  // white
}
)";