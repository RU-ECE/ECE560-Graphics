#include "graphics_core.hh"
#include "shader_texture.hh"
using namespace std;
// Define the external variables declared in graphics_core.hh
app_settings settings = {
	"Tutorial 09: Texture", 1200, 1000, color(0,0,0)
};

class tut09 : public app_base {
private:
    uint32_t programID;
    uint32_t vao;
    uint32_t vbo;
    uint32_t texture;
public:
    void init() override {
        programID = build_prog(vs_texture, fs_texture);
        glUseProgram(programID); // important: shader must be in use to pre-set its parameters
        glUniform1i(glGetUniformLocation(programID, "tex"), 0); // set the texture unit to 0 for this shader
        // this is slow, so do it only once in initialization
        // note: if you don't do this, it still may work, but unreliably based on drive state
        glUseProgram(0); // stop using the shader
        texture = loadWebPTexture("../img/earth.webp");

        const vector<float> xyuv = {
            -0.5f, +0.5f, 0,0,
            -0.5f, -0.5f, 0,1,
            +0.5f, +0.5f, 1,0,
            +0.5f, -0.5f, 1,1
        };
        make_vao_vbo(vao, vbo, xyuv.data(), xyuv.size());
    }
    void render() override {
        bindxyuv(vao, vbo);
        glUseProgram(programID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        unbind(vao, vbo);
    }

    void cleanup() override {
        glDeleteProgram(programID);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteTextures(1, &texture);
    }
};

app_base* app = new tut09();
