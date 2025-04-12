#pragma once

const char vs_fasttext[] = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

const char fs_fasttext[] = R"(
#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

// Texture atlas containing glyphs in a grid
uniform sampler2D fontAtlas;
// SSBO containing the letters to render
layout(std430, binding = 0) buffer LetterBuffer {
    // Each letter contains: ASCII code, position x, position y
    uvec4 letters[];  // Using uvec4 for alignment, last component unused
};

// Uniforms for font metrics
uniform vec2 glyphSize;      // Size of each glyph in texture coordinates
uniform vec2 atlasSize;      // Number of glyphs in each row/column
uniform vec2 screenSize;     // Window dimensions
uniform vec4 textColor;      // Color of the text

void main() {
    // Convert screen coordinates to glyph space
    vec2 pos = gl_FragCoord.xy / screenSize;
    
    // Find which letter we're rendering
    uint letterIndex = uint(floor(pos.x / glyphSize.x));
    if (letterIndex >= letters.length()) {
        discard;
    }
    
    // Get the character code
    uint charCode = letters[letterIndex].x;
    
    // Calculate glyph position in atlas
    uint glyphX = charCode % uint(atlasSize.x);
    uint glyphY = charCode / uint(atlasSize.x);
    
    // Calculate texture coordinates within the glyph
    vec2 glyphOffset = fract(pos / glyphSize);
    vec2 texCoord = (vec2(glyphX, glyphY) + glyphOffset) / atlasSize;
    
    // Sample the texture
    float alpha = texture(fontAtlas, texCoord).r;
    
    // Output the colored glyph
    FragColor = textColor * alpha;
}
)";
