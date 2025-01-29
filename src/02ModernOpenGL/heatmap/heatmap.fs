#version 330 core
#take a patch of points x y val and convert value to a color via 
# render as a flat shaded surface

uniform float valmin;
uniform float valmax;
uniform sampler1D colormap;

in float val;
in vec2 fragCoord;

out vec4 FragColor;

void main() {
    float t = (val - valmin) / (valmax - valmin); // Normalize val to [0, 1]
    vec4 color = texture(colormap, t); // Sample the colormap
    FragColor = vec4(color.rgb, 1.0); // Set the fragment color
}

void main() {
    FragColor = vec4(color, 1.0);
}