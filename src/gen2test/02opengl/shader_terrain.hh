#pragma once
// Terrain: grid mesh (CPU) + height in VS (FBM). Matches logic in shadertoy_example.glsl (planar).

const char* vs_terrain = R"(
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;

uniform mat4 uMVP;

out vec3 vWorldPos;
out float vHeight;

const int   num_octaves = 5;
const float start_amp   = 0.6;
const float decay       = 0.6;
const float scale        = 10.5;
const float warpFreq     = 2.0;
const float warpStrength = 2.0;
const vec3  seed = vec3(12.3, 45.6, 78.9);

float hash(vec3 p) {
    p  = fract(p * vec3(0.1031, 0.11369, 0.13787));
    p += dot(p, p.yxz + 19.19);
    return fract((p.x + p.y) * p.z);
}

float noise(vec3 p) {
    vec3 i = floor(p);
    vec3 f = fract(p);
    vec3 u = f * f * (3.0 - 2.0 * f);
    return mix(
        mix(mix(hash(i+vec3(0,0,0)), hash(i+vec3(1,0,0)), u.x),
            mix(hash(i+vec3(0,1,0)), hash(i+vec3(1,1,0)), u.x), u.y),
        mix(mix(hash(i+vec3(0,0,1)), hash(i+vec3(1,0,1)), u.x),
            mix(hash(i+vec3(0,1,1)), hash(i+vec3(1,1,1)), u.x), u.y),
        u.z);
}

float fbm(vec3 p) {
    float val = 0.0;
    float amp = start_amp;
    float freq = 1.0;
    for (int i = 0; i < num_octaves; i++) {
        val  += amp * noise(p * freq);
        amp  *= decay;
        freq *= 2.0;
    }
    return val;
}

// .x = raw h for coloring (same scale as shadertoy), .y = world elevation
vec2 terrainHeightY(vec2 xz) {
    vec3 q = vec3(xz.x, 0.0, xz.y);
    vec3 warp = vec3(
        fbm(q * warpFreq + seed),
        fbm(q * warpFreq + seed + 10.0),
        fbm(q * warpFreq + seed + 20.0)
    );
    float continents = fbm(q * 1.5 + seed);
    float detail     = fbm(q * scale + warpStrength * warp + seed);
    float h = mix(detail, continents, 0.5);
    h = pow(h, 3.0);
    return vec2(h, h * 0.35);
}

void main() {
    vec2 xz = vec2(pos.x, pos.z) * 2.0;
    vec2 hy = terrainHeightY(xz);
    vHeight = hy.x;
    vec4 wp = vec4(pos.x, hy.y, pos.z, 1.0);
    vWorldPos = wp.xyz;
    gl_Position = uMVP * wp;
}
)";

const char* fs_terrain = R"(
#version 330 core

in vec3 vWorldPos;
in float vHeight;
out vec4 FragColor;

vec3 terrainColor(float h) {
    vec3 deepSea  = vec3(0.02, 0.08, 0.35);
    vec3 shallows = vec3(0.05, 0.35, 0.70);
    vec3 sand     = vec3(0.76, 0.70, 0.50);
    vec3 grass    = vec3(0.18, 0.52, 0.18);
    vec3 rock     = vec3(0.48, 0.43, 0.38);
    vec3 snow     = vec3(0.92, 0.94, 1.00);

    if (h < 0.38) return mix(deepSea,  shallows, h / 0.38);
    if (h < 0.46) return mix(shallows, sand,    (h - 0.38) / 0.08);
    if (h < 0.56) return mix(sand,     grass,   (h - 0.46) / 0.10);
    if (h < 0.72) return mix(grass,    rock,    (h - 0.56) / 0.16);
                  return mix(rock,     snow,    clamp((h - 0.72) / 0.20, 0.0, 1.0));
}

void main() {
    vec3 ddx = dFdx(vWorldPos);
    vec3 ddy = dFdy(vWorldPos);
    vec3 N = normalize(cross(ddx, ddy));

    vec3 lightDir = normalize(vec3(1.2, 0.85, 1.0));
    float diff = max(dot(N, lightDir), 0.0);
    float ambient = 0.22;

    vec3 base = terrainColor(vHeight);
    vec3 col = base * (ambient + diff * 0.78);

    FragColor = vec4(col, 1.0);
}
)";
