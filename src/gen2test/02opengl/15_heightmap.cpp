#include "graphics_core.hh"
#include "shader_heightmap.hh"
#include "terrain.hh"
using namespace std;
// Define the external variables declared in graphics_core.hh
app_settings settings = {
	"Tutorial 15: Terrain Height Map", 1200, 1000, color(0,0,0)
};

class tut15 : public app_base {
private:
    uint32_t programID;
    uint32_t vao;
    uint32_t vbo;
    GLuint texture;

public:
    void init() override {
        programID = build_prog(vs_terrain, fs_terrain);
        make_terrain();
    }
    void render() override {
        bindxyuv(vao, vbo);
        glUseProgram(programID);
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
