#include "graphics_core.hh"
#include "shader_fasttext.hh"
#include <ft2build.h>
#include FT_FREETYPE_H
using namespace std;
// Define the external variables declared in graphics_core.hh
app_settings settings = {
	"Tutorial 14: Fast Text", 1200, 1000, color(0,0,0)
};
class File {
public:
    static char* load(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            return nullptr;
        }
        file.seekg(0, ios::end);
        size_t size = file.tellg();
        file.seekg(0, ios::beg);
        char* buffer = new char[size];
        file.read(buffer, size);
        file.close();
        return buffer;
    }
};

class tut14 : public app_base {
private:
    uint32_t programID;
    uint32_t vao, vbo;
    uint32_t ssbo;  // Storage buffer for text
    GLuint fontAtlas;
    int atlas_width;
    int atlas_height;

    bool load_font(const char* fontname, uint32_t font_size, uint32_t atlas_w = 16, uint32_t atlas_h = 16) {
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            cerr << "Could not init FreeType Library" << endl;
            return false;
        }

        FT_Face face;
        if (FT_New_Face(ft, fontname, 0, &face)) {
            cerr << "Failed to load font: " << fontname << endl;
            FT_Done_FreeType(ft);
            return false;
        }

        FT_Set_Pixel_Sizes(face, 0, font_size);

        // Calculate atlas dimensions
        atlas_width = atlas_w * font_size;
        atlas_height = atlas_h * font_size;
        vector<unsigned char> atlas_data(atlas_width * atlas_height, 0);

        // Generate texture atlas
        glGenTextures(1, &fontAtlas);
        glBindTexture(GL_TEXTURE_2D, fontAtlas);

        // For each ASCII character we want to support
        for (unsigned char c = 32; c < 128; c++) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                cerr << "Failed to load glyph: " << c << endl;
                continue;
            }

            // Calculate position in atlas
            int x = (c % atlas_w) * font_size;
            int y = (c / atlas_w) * font_size;

            // Copy glyph bitmap to atlas
            FT_GlyphSlot glyph = face->glyph;
            for (unsigned int row = 0; row < glyph->bitmap.rows; row++) {
                for (unsigned int col = 0; col < glyph->bitmap.width; col++) {
                    int atlas_offset = (y + row) * atlas_width + (x + col);
                    if (atlas_offset < atlas_data.size()) {
                        atlas_data[atlas_offset] = 
                            glyph->bitmap.buffer[row * glyph->bitmap.width + col];
                    }
                }
            }
        }

        // Upload texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 
                    atlas_width, atlas_height,
                    0, GL_RED, GL_UNSIGNED_BYTE, 
                    atlas_data.data());

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Cleanup
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        return true;
    }

public:
    void init() override {
        programID = build_prog(vs_fasttext, fs_fasttext);
        
        // Load text
        char* text = File::load("data/warandpeace.txt");
        size_t textSize = strlen(text);
        
        // Create and setup SSBO for text data
        glGenBuffers(1, &ssbo);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, textSize, text, GL_STATIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
        
        delete[] text;

        // Setup screen quad
        const vector<float> xyuv = {
            -1, +1, 0,0,
            -1, -1, 0,1,
            +1, +1, 1,0,
            +1, -1, 1,1
        };
        make_vao_vbo(vao, vbo, xyuv.data(), xyuv.size());
        
        // TODO: Load or create font atlas texture
        // For now, using a placeholder
        glGenTextures(1, &fontAtlas);
    }

    void render() override {
        bindxyuv(vao, vbo);
        glUseProgram(programID);
        glBindTexture(GL_TEXTURE_2D, fontAtlas);
        
        // Set uniforms
        glUniform2f(glGetUniformLocation(programID, "glyphSize"), 0.1f, 0.1f);
        glUniform2f(glGetUniformLocation(programID, "atlasSize"), 16.0f, 16.0f);
        glUniform2f(glGetUniformLocation(programID, "screenSize"), 1200.0f, 1000.0f);
        glUniform4f(glGetUniformLocation(programID, "textColor"), 1.0f, 1.0f, 1.0f, 1.0f);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        unbind(vao, vbo);
    }

    void cleanup() override {
        glDeleteProgram(programID);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ssbo);
        glDeleteTextures(1, &fontAtlas);
    }
};

app_base* app = new tut14();
