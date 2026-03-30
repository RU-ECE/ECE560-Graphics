void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    vec2 uv = fragCoord / iResolution.y;

    const float R = 0.1; // radii of circles
    const float d = 2.0 * R; // diameter
    const float thickness = 0.005;

    // position within each tile, centered at (R, R)
    float cx = mod(uv.x, d) - R; // center of circle is (-R,-R)
    float cy = mod(uv.y, d) - R;

    float dist = sqrt(cx*cx + cy*cy);  // distance from circle center

    vec3 lineColor = vec3(0.2, 0.6, 1.0);
    vec3 bgColor   = vec3(0.05, 0.05, 0.1);

    // draw ring: only where dist is near R
    vec3 col = abs(dist - R) < thickness ? lineColor : bgColor;

    fragColor = vec4(col, 1.0);
}