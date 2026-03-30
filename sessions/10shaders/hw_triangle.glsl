void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    vec2 uv = fragCoord / iResolution.y;

    const float S = 0.1; // side of triangle
    const float H = S*sqrt(3.0)/2.0; // height of triangle
    const float thickness = 0.005;



    vec3 lineColor = vec3(0.2, 0.6, 1.0);
    vec3 bgColor   = vec3(0.05, 0.05, 0.1);

    vec3 col = mod(uv.y, H) <= thickness ? lineColor : bgColor;

    fragColor = vec4(col, 1.0);
}
