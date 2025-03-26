#include "graphics_core.hh"
#include "shader_simple.hh"

// Define the external variables declared in graphics_core.hh
app_settings settings = {
	"Tutorial 02: single color Triangle", 1200, 1000, color(0.6,0,0)
};

class OpenGLWindow : public app_base {
private:
	uint32_t programID;
	uint32_t vao;
    uint32_t vbo;
public:
	void init() override {
        programID = build_prog(vs_simple, fs_simple);

        // Create and bind the VAO and VBO
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        const float triangle_vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };
        // Upload the vertex data to the VBO
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

        // Set up the vertex attribute pointer for position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
	}
	
	void render() override {
        glUseProgram(programID);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
	}

	void cleanup() override {
		glDeleteProgram(programID);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}
};

app_base* app = new OpenGLWindow();
