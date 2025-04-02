#include "graphics_core.hh"
#include "shader_gouraud.hh"

using namespace std;
// Define the external variables declared in graphics_core.hh
app_settings settings = {
	"Tutorial 05: Rectangle with GL_TRIANGLE_STRIP", 1200, 1000, color(0,0,0)
};

class OpenGLWindow : public app_base {
private:
    uint32_t programID;
    uint32_t vao;
    uint32_t vbo;
public:
    void init() override {
        programID = build_prog(vs_gouraud, fs_common);

        const vector<float> vert = {
            -0.5f, +0.5f, 1,0,0,
            -0.5f, -0.5f, 0,1,0,
            +0.5f, +0.5f, 0,0,1,
            +0.5f, -0.5f, 0,0,1
        };
        make_vao_vbo(vao, vbo, vert.data(), vert.size());
    }
	
    void render() override {
        glUseProgram(programID);
        bindxyrgb(vao, vbo);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        unbind(vao, vbo);
    }

    void cleanup() override {
        glDeleteProgram(programID);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
};

app_base* app = new OpenGLWindow();
