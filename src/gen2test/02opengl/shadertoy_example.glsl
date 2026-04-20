/*============================================================
  PROCEDURAL PLANET (cleaned)
  Author: Dov Kruger

===========================================================*/

const int   num_octaves = 3; // the range of scales of noise
const float start_amp   = 0.6; // the top level amplitude
const float decay       = 0.75;

const float scale        = 10.5;
const float warpFreq     = 2.0;
const float warpStrength = 2.0;
float minDist = 1.02;   // just above surface (radius = 1)
float maxDist = 2.5;
float speed   = 0.2;

const vec3  seed = vec3(12.3, 45.6, 78.9);
//const vec3  seed = vec3(4.2, 15.6, 81.9);

// ------ Hash ------------------------------------------------
float hash(vec3 p) {
    p  = fract(p * vec3(0.1031, 0.11369, 0.13787));
    p += dot(p, p.yxz + 19.19);
    return fract((p.x + p.y) * p.z);
}

// ------ Value noise -----------------------------------------
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

// ------ FBM -------------------------------------------------
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

// ------ Rotations -------------------------------------------
mat3 rotY(float a) {
    float c = cos(a), s = sin(a);
    return mat3( c, 0, s,
                 0, 1, 0,
                -s, 0, c);
}
mat3 rotX(float a) {
    float c = cos(a), s = sin(a);
    return mat3(1, 0, 0,
                0, c,-s,
                0, s, c);
}

// ------ Ray / sphere ----------------------------------------
float hitSphere(vec3 ro, vec3 rd, float r) {
    float b = dot(ro, rd);
    float c = dot(ro, ro) - r * r;
    float h = b*b - c;
    if (h < 0.0) return -1.0;
    return -b - sqrt(h);
}

// ------ Terrain color ---------------------------------------
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

// ============================================================
void mainImage(out vec4 fragColor, in vec2 fragCoord) {

    // --- camera ---
    vec2 uv = (fragCoord - 0.5 * iResolution.xy) / iResolution.y;
    vec3 ro = vec3(0.0, 0.0, 2.5);
    vec3 rd = normalize(vec3(uv, -1.8));

    // --- mouse rotation ---
    vec2 mouse = iMouse.xy / iResolution.xy;
    if (iMouse.z < 0.5) mouse = vec2(0.35, 0.45);

    float yaw   = (mouse.x - 0.5) * 6.283;
    float pitch = (mouse.y - 0.5) * 3.14;

    mat3 rot = rotX(-pitch) * rotY(-yaw);

    vec3 col = vec3(0.02, 0.02, 0.06);

    float t = hitSphere(ro, rd, 1.0);
    if (t > 0.0) {

        vec3 hitPos = ro + t * rd;
        vec3 N      = normalize(hitPos);
        vec3 q      = rot * N;

        // --- domain warp ---
        vec3 warp = vec3(
            fbm(q * warpFreq + seed),
            fbm(q * warpFreq + seed + 10.0),
            fbm(q * warpFreq + seed + 20.0)
        );

        // --- height ---
        float continents = fbm(q * 1.5 + seed);
        float detail     = fbm(q * scale + warpStrength * warp + seed);

        float h = mix(detail, continents, 0.5);
        h = pow(h, 3.0);

        // --- lighting ---
        vec3 lightDir = normalize(vec3(1.2, 0.6, 1.0));
        float diff    = max(dot(N, lightDir), 0.0);
        float ambient = 0.18;

        vec3 viewDir  = -rd;
        vec3 halfVec  = normalize(lightDir + viewDir);
        float spec    = (h < 0.46)
                      ? pow(max(dot(N, halfVec), 0.0), 64.0) * 0.6
                      : 0.0;

        col = terrainColor(h) * (ambient + diff * 0.82) + spec;

        float rim = 1.0 - max(dot(N, viewDir), 0.0);
        col += vec3(0.1, 0.2, 0.5) * pow(rim, 4.0) * 0.6;
    }

    fragColor = vec4(col, 1.0);
}