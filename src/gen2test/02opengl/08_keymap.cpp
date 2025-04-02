#include "graphics_core.hh"
#include "shader_gouraudtransform.hh"

using namespace std;
// Define the external variables declared in graphics_core.hh
app_settings settings = {
	"Tutorial 08: Key Maps", 1200, 1000, color(0,0,0)
};

class OpenGLWindow : public app_base {
private:
    uint32_t programID;
    uint32_t vao;
    uint32_t vbo;
    glm::mat4 m1, m2;
public:
    void init() override {
        std_3d_camera_controls(); // Initialize the camera controls

        programID = build_prog(vs_gouraudtransform, fs_common);

        const vector<float> vert = {
            -0.5f, -0.5f, 1,0,0,
            +0.5f, -0.5f, 0,1,0,
             0.0f, +0.5f, 0,0,1
        };
        make_vao_vbo(vao, vbo, vert.data(), vert.size());
        m1 = glm::scale(glm::mat4(1.0f), glm::vec3(1.2f, 0.8f, 1.0f));
        m2 = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    }
    void pan_left() {
        cerr << "pan_left" << endl;
        m1 = glm::translate(m1, glm::vec3(-0.1f, 0.0f, 0.0f));
    }
    void pan_right() {
        cerr << "pan_right" << endl;
        m1 = glm::translate(m1, glm::vec3(0.1f, 0.0f, 0.0f));
    }
    void render() override {
        bindxyrgb(vao, vbo);
        glUseProgram(programID);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        int modelLoc = glGetUniformLocation(programID, "transform");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &m1[0][0]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &m2[0][0]);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        unbind(vao, vbo);
    }

    void cleanup() override {
        glDeleteProgram(programID);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
};

app_base* app = new OpenGLWindow();
