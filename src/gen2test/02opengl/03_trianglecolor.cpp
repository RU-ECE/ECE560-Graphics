#include "graphics_core.hh"
#include "shader_onecolor.hh"

using namespace std;
// Define the external variables declared in graphics_core.hh
app_settings settings = {
	"Tutorial 03: color parameter Triangle", 1200, 1000, color(0.6,0,0) //red background
};

class OpenGLWindow : public app_base {
private:
    uint32_t programID;
    uint32_t vao;
    uint32_t vbo;
public:
    void init() override {
        programID = build_prog(vs_onecolor, fs_onecolor);

        const static vector<float> triangle_vertices = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.0f,  0.5f
        };
        // Create and bind the VAO and VBO
        make_vao_vbo(vao, vbo, triangle_vertices.data(), triangle_vertices.size());
    }

    void render() override {
        glUseProgram(programID);
        GLint colorLoc = glGetUniformLocation(programID, "color");
        glUniform3f(colorLoc, 0.0f, 0.1f, 0.6f);  // Dark blue triangle

        bindxy(vao, vbo);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        unbind(vao, vbo);
    }

    void cleanup() override {
        glDeleteProgram(programID);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
};

app_base* app = new OpenGLWindow();
