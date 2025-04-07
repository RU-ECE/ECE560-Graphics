#include "graphics_core.hh"
#include "shader_texture.hh"
#include "mesh.hh"

using namespace std;
// Define the external variables declared in graphics_core.hh
app_settings settings = {
	"Tutorial 10: Load Model", 1200, 1000, color(0,0,0)
};

class OpenGLWindow : public app_base {
private:
    uint32_t programID;
    uint32_t vao;
    uint32_t vbo;
    uint32_t texture;
    mesh* m;
public:
    void init() override {
        programID = build_prog(vs_texture, fs_texture);
        texture = loadWebPTexture("models/wal67ar_small.jpg");
        blockloader bl("models/buddha.blobj");
        m = new mesh(bl);
        make_vao_vbo(vao, vbo, &m->vertices[0], m->vertices.size());
    }
	
    void render() override {
        glUseProgram(programID);
        bindxyrgb(vao, vbo);
        glDrawArrays(GL_TRIANGLES, 0, m->vertices.size());
        unbind(vao, vbo);
    }

    void cleanup() override {
        glDeleteProgram(programID);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
};

app_base* app = new OpenGLWindow();
