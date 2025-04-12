#include "graphics_core.hh"
#include "shader_texture.hh"
#include "shader_mandelbrot.hh"
using namespace std;
// Define the external variables declared in graphics_core.hh
app_settings settings = {
	"Tutorial 12: Dynamic Mandelbrot Texture", 1200, 1000, color(0,0,0)
};

class tut12 : public app_base {
private:
    uint32_t programID;
    uint32_t vao;
    uint32_t vbo;
    GLuint texture;
    GLuint colormap;
public:
    void init() override {
        programID = build_prog(vs_mandelbrot, fs_mandelbrot);

        // Create simple 1D gradient texture
        glGenTextures(1, &colormap);
        glBindTexture(GL_TEXTURE_1D, colormap);
        float colors[] = { 1,0,0, 0,0,1 };  // red to blue
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 2, 0, GL_RGB, GL_FLOAT, colors);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        const vector<float> xy = {
            -0.5f, +0.5f,
            -0.5f, -0.5f,   
            +0.5f, +0.5f,
            +0.5f, -0.5f,
        };
        make_vao_vbo(vao, vbo, xy.data(), xy.size());
    }
    void render() override {
        bindxy(vao, vbo);
        glUseProgram(programID);
        glUniform1f(glGetUniformLocation(programID, "xmin"), -2.0f);
        glUniform1f(glGetUniformLocation(programID, "xmax"), 1.0f);
        glUniform1f(glGetUniformLocation(programID, "ymin"), -1.5f);
        glUniform1f(glGetUniformLocation(programID, "ymax"), 1.5f);
        glUniform1i(glGetUniformLocation(programID, "colormap"), 0);
        glUniform1i(glGetUniformLocation(programID, "maxiter"), 1000);
        glBindTexture(GL_TEXTURE_1D, colormap);
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

app_base* app = new tut12();
