#version 330 core

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec3 Normal;

layout(location = 0) out vec4 FragColor;

// Material parameters
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;

uniform vec3 lightDir;   // Directional light direction (should be normalized)
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main() {
    // Normalize input normal
    vec3 norm = normalize(Normal);
    
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse
    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
