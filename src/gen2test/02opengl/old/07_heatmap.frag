#version 330 core
out vec4 FragColor;

//in vec2 TexCoord;
in float val;
//uniform sampler2D ourTexture;
uniform vec3 minColor;
uniform vec3 maxColor;
uniform float minVal;
uniform float maxVal;
float b = 1.0 / (maxVal - minVal);
float a = -minVal * b;


void main()
{
  //float f = (val - minVal) / (maxVal-minVal);
	float f = val * b + a; //optimized by precomputing coefficients, only one multiply and add
	FragColor = vec4(minColor*(1.0-f) + maxColor * f,1.0);
}
