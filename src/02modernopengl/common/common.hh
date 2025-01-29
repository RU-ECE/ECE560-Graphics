#include <GL/glew.h>    // OpenGL API
#include <GLFW/glfw3.h> // Window API
#include <glm/glm.hpp>  // Matrix and vector math for OpenGL
#include <glm/ext.hpp>

// all demos use a window, declared globally in common.cc
extern GLFWwindow* win;
GLFWwindow* createWindow(uint32_t w, uint32_t h, const char title[]);
GLuint loadShaders(const char vertexPath[], const char * fragmentPath);

// the signature of the function to write, automatically called by main
void glmain();

/*
	we provide a standardized main, because it's always the same
  It catches exceptions and quits if there is a problem
	you write glmain instead
*/
int main(int argc, char* argv[]);
void dump(glm::mat4& mat);
void transpt(glm::mat4& m, double x, double y, double z);
void transform_dump(glm::mat4& mat, double x, double y, double z);
GLuint loadWebPTexture(const char* filePath);
GLuint build_prog(const char vertex_shader[], const char fragment_shader[]);
uint32_t make_static_vbo(const float vert[], uint32_t len);

const uint32_t INVALID_UNIFORM_LOCATION = 0xFFFFFFFF;


// TODO: perhaps each function should have a hardcoded, prebuilt shader program
/*
  generically rednder a textured object composed of a VAO, containing a vertex buffer, an index buffer,
   a texture, and a texture unit
*/
void render_textured_indexed(GLuint program, GLuint vao, GLuint vert, GLuint index, GLuint texture);

/*
  generically rednder a textured object composed of a VAO, containing a vertex buffer with a color per vertex,
   an index buffer,
*/
void render_indexed_colored(GLuint program, GLuint vao, GLuint vert, GLuint index);

/*
	generically render a surface composed of a VAO, containing a vertex buffer with a value per vertex looking up in a 1D-texture	
*/
void render_indexed_heatmap(GLuint program, GLuint vao, GLuint vert, GLuint index, GLuint texture);


extern const char* const vertex_shader_pervertex;
extern const char* const frag_shader_common;