#include "graphics_core.hh"
#include "shader_texture_transform.hh"
using namespace std;
// Define the external variables declared in graphics_core.hh
app_settings settings = {
	"Tutorial 13: Texture", 1200, 1000, color(0,0,0)
};

class tut13 : public app_base {
private:
    uint32_t programID;
    uint32_t vao;
    uint32_t vbo;
    GLuint face1, face2, face3, face4, face5, face6; // 6 separate textures
    float angle;
public:
    void init() override {
        programID = build_prog(vs_texture, fs_texture);
        const string dir = "../img/";
        face1 = loadWebPTexture((dir + "birthday1.webp").c_str());
        face2 = loadWebPTexture((dir + "gnu_linux.webp").c_str());
        face3 = loadWebPTexture((dir + "galaxy.webp").c_str());
        face4 = loadWebPTexture((dir + "birthday2.webp").c_str());
        face5 = loadWebPTexture((dir + "flowers1.webp").c_str());
        face6 = loadWebPTexture((dir + "flowers2.webp").c_str());
        angle = 0.0f;
        const vector<float> square_xyuv = {
            // x    y    z    u    v
            -0.5f, -0.5f, 0.0f, 1.0f,  // bottom left
            -0.5f,  0.5f, 0.0f, 0.0f,  // top left
            +0.5f, -0.5f, 1.0f, 1.0f,  // bottom right
            +0.5f,  0.5f, 1.0f, 0.0f   // top right
        };
        make_vao_vbo(vao, vbo, square_xyuv.data(), square_xyuv.size());
    }
    void render() override {
        bindxyuv(vao, vbo);
        glUseProgram(programID);
        int transformLoc = glGetUniformLocation(programID, "transform");
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        // Front face
        glm::mat4 transform = glm::translate(rot, glm::vec3(0.0f, 0.0f, 0.5f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);
        glBindTexture(GL_TEXTURE_2D, face1);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Back face
        transform = glm::rotate(glm::translate(rot, glm::vec3(0.0f, 0.0f, -0.5f)), 
                              glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);
        glBindTexture(GL_TEXTURE_2D, face2);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Right face
        transform = glm::rotate(glm::translate(rot, glm::vec3(0.5f, 0.0f, 0.0f)),
                              glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);
        glBindTexture(GL_TEXTURE_2D, face3);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Left face
        transform = glm::rotate(glm::translate(rot, glm::vec3(-0.5f, 0.0f, 0.0f)),
                              -glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);
        glBindTexture(GL_TEXTURE_2D, face4);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Top face
        transform = glm::rotate(glm::translate(rot, glm::vec3(0.0f, 0.5f, 0.0f)),
                              -glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);
        glBindTexture(GL_TEXTURE_2D, face5);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Bottom face
        transform = glm::rotate(glm::translate(rot, glm::vec3(0.0f, -0.5f, 0.0f)),
                              glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);
        glBindTexture(GL_TEXTURE_2D, face6);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        unbind(vao, vbo);
        angle += 0.01f;
    }

    void cleanup() override {
        glDeleteProgram(programID);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteTextures(1, &face1);
        glDeleteTextures(1, &face2);
        glDeleteTextures(1, &face3);
        glDeleteTextures(1, &face4);
        glDeleteTextures(1, &face5);
        glDeleteTextures(1, &face6);
    }
};

app_base* app = new tut13();
