#include <vector>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>        // GLEW must come first!
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>


using namespace std;


GLFWwindow* win = nullptr;


void check_status( GLuint obj, bool isShader ) {
    GLint status = GL_FALSE, log[ 1 << 11 ] = { 0 };
    ( isShader ? glGetShaderiv : glGetProgramiv )( obj, isShader ? GL_COMPILE_STATUS : GL_LINK_STATUS, &status );
    if( status == GL_TRUE ) return;
    ( isShader ? glGetShaderInfoLog : glGetProgramInfoLog )( obj, sizeof( log ), nullptr, (GLchar*)log );
    std::cerr << (GLchar*)log << "\n";
    std::exit( EXIT_FAILURE );
}

void attach_shader( GLuint program, GLenum type, const char* src ) {
    GLuint shader = glCreateShader( type );
    glShaderSource( shader, 1, &src, NULL );
    glCompileShader( shader );
    check_status( shader, true );
    glAttachShader( program, shader );
    glDeleteShader( shader );
}

// build a vertex and fragment shader program from constants in source code
// this hardcoded version is provided for all the standard shaders that you want for common graphics
// you can always load from files but I will build a number of the basic ones in here
GLuint build_prog(const char vertex_shader[], const char fragment_shader[]) {
    GLuint prog = glCreateProgram();
    attach_shader( prog, GL_VERTEX_SHADER, vertex_shader );
    attach_shader( prog, GL_FRAGMENT_SHADER, fragment_shader );
    glLinkProgram( prog );
    check_status( prog, false );
	return prog;
}

const char* const vertex_shader_pervertex = R"GLSL(
#version 330 core
layout ( location = 0 ) in vec2 pos;
layout ( location = 1 ) in vec3 Color;
uniform mat4 transform;

out VertexData
{
    vec3 Color;
} vsOutput;
void main()
{
    gl_Position = transform*vec4(pos, 0.0,1.0);
    vsOutput.Color = Color;
}
)GLSL";

// this fragment shader can be used for any vertex shader that uses rgb colors stored per vertex
const char* const frag_shader_common = R"GLSL(
#version 330 core
in VertexData
{
    vec3 Color;
} fsInput;
out vec4 outColor;
void main()
{
    outColor = vec4( fsInput.Color, 1.0 );
}
)GLSL";
uint32_t make_static_vbo(const float vert[], uint32_t len) {
	uint32_t vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, len, vert, GL_STATIC_DRAW);
	return vbo;
}

uint32_t make_vao() {
	uint32_t vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	return vao;
}

GLFWwindow* createWindow(uint32_t w, uint32_t h, const char title[]) {
	// Initialise GLFW
	if( !glfwInit() )	{
		throw "Failed to initialize GLFW";
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	GLFWwindow* win = glfwCreateWindow(w, h, title, nullptr, nullptr);
	if (win == nullptr) {
		glfwTerminate();
		throw "Failed to open GLFW window";
	}
	glfwMakeContextCurrent(win); // create OpenGL context

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		throw "Failed to initialize GLEW";
	}

	// Ensure we can capture the escape key to quit
	glfwSetInputMode(win, GLFW_STICKY_KEYS, GL_TRUE);

	return win;
}

void bindxyrgb(uint32_t vao) {
	glBindVertexArray(vao);           // draw using vao and its vbo, and anything else inside it
	glVertexAttribPointer(
		0,                  // first parameter to shader, numbered 0
		2,                  // 2 floating point numbers (x,y) z = 0
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		5*sizeof(float),    // there are 5 numbers total, this uses first 2
		(void*)0            // array buffer offset
	);
	glVertexAttribPointer(
		1,                     // 2nd parameter to shader, numbered 1
		3,                     // 3 floating point numbers r,g,b
		GL_FLOAT,              // all these values are float
		GL_FALSE,              // normalized?
		5*sizeof(float),       // there are 5 numbers total, this uses first 2
		(void*)(2*sizeof(float)) // after x,y offset of rgb = 2
	);

	glEnableVertexAttribArray(0); // pass x,y to shader
	glEnableVertexAttribArray(1); // pass rgb to shader
}

void unbind() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
  	glBindVertexArray(0);            // stop drawing using vao
}

void cleanup(uint32_t vao, uint32_t vbo, uint32_t programID) {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);
}

/*
	standardized main to catch errors.
	In this simplified version each error is just reported as a string
	It would be better to also track which file and line number the error
	happened in, but that would take an exception object.
	For now, keeping it simple
 */
int main(int argc, char* argv[]) {
	try {
		win = createWindow(1000, 800, "OpenGL Demo"); // Move this to the start
		
		static const float vertices[] = { 	// each point is x,y, r,g,b
			-1.0f, -1.0f,    1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f,    0.0f, 1.0f, 0.0f,
			 0.0f,  1.0f,    0.0f, 0.0f, 1.0f,
		};

		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		uint32_t programID = build_prog(vertex_shader_pervertex, frag_shader_common);
		uint32_t vao = make_vao();
		uint32_t vbo = make_static_vbo(vertices, sizeof(vertices));
		float eyex, eyey = 0, eyez;
		float viewing_radius = 5;
		float viewing_angle = 0;
		float a1 = 0;
		do {
			glClear( GL_COLOR_BUFFER_BIT );  	// Clear the screen
			glUseProgram(programID);      		// Use our shader
			bindxyrgb(vao);

			glm::mat4 t1 = glm::scale(glm::identity<glm::mat4>(), glm::vec3(0.5,0.5,1));
			glm::mat4 t2 = glm::translate(t1, glm::vec3(2,0,0));
			t1 = glm::rotate(t1, a1, glm::vec3(0,1,0));
			//t2 = glm::rotate(t2, a2, glm::vec3(0,1,0));
			glm::mat4 proj = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
			eyex = viewing_radius*cos(viewing_angle);
			eyez = viewing_radius*sin(viewing_angle);
			glm::mat4 view = glm::lookAt(glm::vec3(eyex,eyey,eyez), glm::vec3(0,0,0), glm::vec3(0,1,0));
			glm::mat4 transform =  proj * (view * t1);
			uint32_t matrixID = glGetUniformLocation(programID, "transform");
			glUniformMatrix4fv(matrixID, 1, GL_FALSE, &transform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
			transform = proj * (view * t2);
			glUniformMatrix4fv(matrixID, 1, GL_FALSE, &transform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
			unbind();
			glfwSwapBuffers(win); // double buffer
			a1 += .01;
			viewing_angle += .01;
			glfwPollEvents();
		}	while( glfwGetKey(win, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
				   glfwWindowShouldClose(win) == 0 );
		cleanup(vao, vbo, programID);
		glfwTerminate();		// Close OpenGL window and terminate GLFW
	} catch (const char* msg) {
		cerr << msg << '\n';
		exit(-1);
	}
	return 0;
}
