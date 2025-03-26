#include "graphics_core.hh"
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <glm/glm.hpp>
#include <webp/decode.h>
using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow* win = nullptr;

app_base::~app_base() {}  // Define virtual destructor

void check_status( GLuint obj, bool isShader ) {
    GLint status = GL_FALSE, log[ 1 << 11 ] = { 0 };
    ( isShader ? glGetShaderiv : glGetProgramiv )( obj, isShader ? GL_COMPILE_STATUS : GL_LINK_STATUS, &status );
    if( status == GL_TRUE ) return;
    ( isShader ? glGetShaderInfoLog : glGetProgramInfoLog )( obj, sizeof( log ), nullptr, (GLchar*)log );
    std::cerr << (GLchar*)log << "\n";
    std::exit( EXIT_FAILURE );
}

inline void attach_shader( GLuint program, GLenum type, const char* src ) {
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

void bindxy(uint32_t vao, uint32_t vbo) {
	glBindVertexArray(vao);           // draw using vao and its vbo, and anything else inside it
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		0,                  // first parameter to shader, numbered 0
		2,                  // 2 floating point numbers (x,y) z = 0
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		2*sizeof(float),    // there are 5 numbers total, this uses first 2
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0); // pass x,y to shader
}

void bindxyrgb(uint32_t vao, uint32_t vbo) {
	glBindVertexArray(vao);           // draw using vao and its vbo, and anything else inside it
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
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

void bindxyzrgb(uint32_t vao, uint32_t vbo) {
	glBindVertexArray(vao);           // draw using vao and its vbo, and anything else inside it
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		0,                  // first parameter to shader, numbered 0
		3,                  // 3 floating point numbers (x,y,z)
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		6*sizeof(float),    // there are 6 numbers total, this uses first 3
		(void*)0            // array buffer offset
	);
	glVertexAttribPointer(
		1,                     // 2nd parameter to shader, numbered 1
		3,                     // 3 floating point numbers r,g,b
		GL_FLOAT,              // all these values are float
		GL_FALSE,              // normalized?
		6*sizeof(float),       // there are 6 numbers total, this uses first 3
		(void*)(3*sizeof(float)) // after x,y,z offset of rgb = 3
	);

	glEnableVertexAttribArray(0); // pass x,y to shader
	glEnableVertexAttribArray(1); // pass rgb to shader
}

void bindxyzuv(uint32_t vao, uint32_t vbo) {
	glBindVertexArray(vao);           // draw using vao and its vbo, and anything else inside it
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
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

void unbind(uint32_t vao, uint32_t vbo) {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
  	glBindVertexArray(0);            // stop drawing using vao
	glBindBuffer(GL_ARRAY_BUFFER, 0); // stop drawing using vbo
}


/**
 * Create a window and initialize OpenGL context
 * @param w width of the window
 * @param h height of the window
 * @param title title of the window
 * @param major major version of OpenGL by default 3.3 which is fairly old
 * @param minor minor version of OpenGL 
 * @return pointer to the window
 */
GLFWwindow* createWindow(uint32_t w, uint32_t h, const char title[], uint32_t major = 3, uint32_t minor = 3) {
	// Initialise GLFW
	if( !glfwInit() )	{
		throw "Failed to initialize GLFW";
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
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

/*
	standardized main to catch errors.
	In this simplified version each error is just reported as a string
	It would be better to also track which file and line number the error
	happened in, but that would take an exception object.
	For now, keeping it simple
 */
int main(int argc, char* argv[]) {
	try {
        win = createWindow(settings.width, settings.height, settings.title.c_str());
        glClearColor(settings.bg_color.r, settings.bg_color.g, settings.bg_color.b, 1.0f);
        app->init();
        while (!glfwWindowShouldClose(win)) {
            glClear(GL_COLOR_BUFFER_BIT);
            app->render();
            glfwSwapBuffers(win);
            glfwPollEvents();
        }
		glfwTerminate();
	} catch (const char* msg) {
		cerr << msg << '\n';
		exit(-1);
	}
	return 0;
}


void dump(glm::mat4& mat) {
    // TODO: I suspect we are printing the matrix transposed
    const float* m = &mat[0][0];
    cerr << setprecision(7);
    for (int i = 0, c = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++, c++)
        cerr << setw(14) << m[c];
      cerr << '\n';
    }
  }

void transpt(glm::mat4& m, double x, double y, double z) {
    cerr << "orig=(" << x << "," << y << "," << z << ") transformed: (" <<
     (m[0][0] * x + m[1][0] * y + m[2][0] * z + m[3][0]) << "," <<
     (m[0][1] * x + m[1][1] * y + m[2][1] * z + m[3][1]) << "," <<
     (m[0][2] * x + m[1][2] * y + m[2][2] * z + m[3][2]) << ")\t(";

    cerr <<
		 (m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3]) << "," <<
     (m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3]) << "," <<
     (m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3]) << ")\n";

  }

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

void transform_dump(glm::mat4& mat, double x, double y, double z) {
    glm::vec4 point = glm::vec4(x, y, z, 1.0f);
    glm::vec4 tp = mat * point;
    cerr << "Transformed point: (" << tp.x << ", " << tp.y << ", " << tp.z << ")\n";
}

uint32_t make_static_vbo(const float vert[], uint32_t len) {
	uint32_t vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, len, vert, GL_STATIC_DRAW);
	return vbo;
}
