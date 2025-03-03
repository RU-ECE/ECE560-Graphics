#pragma once
/*
    Author: Dov Kruger
    Date: 2025-02-26
    This header file contains all declarations that each demo needs.
    This dramatically simplifies the code for each demo.
    In order to maximize simplicity, this is a header file.
    This means that each demo must include this file, and only once.
    To make a better, though more complicated system we could separate out header and implementation.
    For now, keeping it simple.
*/
#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <GL/glew.h>        // GLEW must come first!
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <webp/decode.h>
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

void bindxyzuv(uint32_t vao) {
	glBindVertexArray(vao);           // draw using vao and its vbo, and anything else inside it
	glVertexAttribPointer(
		0,                  // first parameter to shader, numbered 0
		3,                  // 2 floating point numbers (x,y) z = 0
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		5*sizeof(float),    // there are 5 numbers total, this uses first 2
		(void*)0            // array buffer offset
	);
	glVertexAttribPointer(
		1,                     // 2nd parameter to shader, numbered 1
		2,                     // 3 floating point numbers r,g,b
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

const double PI = M_PI;
GLFWwindow* win = nullptr;

const float triangle_xyrgb[] = {
	-1.0f, -1.0f,    1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f,    0.0f, 1.0f, 0.0f,
	 0.0f,  1.0f,    0.0f, 0.0f, 1.0f,
};

class camera_3d {
private:
    glm::vec3 eye;
    glm::vec3 look_at;
    glm::vec3 up;
    glm::mat4 proj;
    glm::mat4 view;
public:
// start looking from eye to look_at, with up as the up direction, and field of view fov in degrees
    camera_3d(glm::vec3 eye, glm::vec3 look_at, glm::vec3 up, float fov = 45.0f) {
        proj = glm::perspective(glm::radians(fov), 4.0f / 3.0f, 0.1f, 100.0f);
    }
    void update() {
        view = glm::lookAt(eye, look_at, up);
    }

    // override this function to generate different 3d cameras that fly around
    virtual void animate(float dt) = 0;
#if 0
    void view(uint32_t location, const glm::mat4& transform) {
        glm::mat4 m = proj * view * transform;
        glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]);
    }
#endif
};

GLuint loadWebPTexture(const char* filePath) {
    // Read the file into a buffer
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open WebP file: " << filePath << std::endl;
        return 0;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        std::cerr << "Failed to read WebP file: " << filePath << std::endl;
        return 0;
    }

    // Decode the WebP image
    int width, height;
    uint8_t* data = WebPDecodeRGBA(reinterpret_cast<uint8_t*>(buffer.data()), size, &width, &height);
    if (!data) {
        std::cerr << "Failed to decode WebP image: " << filePath << std::endl;
        return 0;
    }

    // Generate and bind a texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Upload the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Free the image data
    WebPFree(data);

    return textureID;
}

void glmain(GLFWwindow* win);

int main(int argc, char* argv[]) {
	try {
		win = createWindow(1000, 800, "OpenGL Demo"); // Move this to the start
		glmain(win);
		glfwTerminate();		// Close OpenGL window and terminate GLFW
	} catch (const char* msg) {
		std::cerr << msg << '\n';
		exit(-1);
	}
	return 0;
}

