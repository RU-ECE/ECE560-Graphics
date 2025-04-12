#include "graphics_core.hh"
#include "shader_fasttext.hh"
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
