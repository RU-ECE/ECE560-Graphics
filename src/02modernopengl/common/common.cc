#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <webp/decode.h>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "common.hh"

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
const char* const vertex_shader_pervertex = 1 + R"GLSL(
#version 330 core
layout ( location = 0 ) in vec2 Position;
layout ( location = 1 ) in vec3 Color;
out VertexData
{
    vec3 Color;
} vsOutput;
void main()
{
    gl_Position = vec4( Position, 0.0, 1.0 );
    vsOutput.Color = Color;
}
)GLSL";

// this fragment shader can be used for any vertex shader that uses rgb colors stored per vertex
const char* const frag_shader_common = 1 + R"GLSL(
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


// fragment shader for a textured material with lighting
const char* const frag_shader_lighting = 1 + R"GLSL(
#version 330

in vec2 TexCoord0;
in vec3 Normal0;

out vec4 FragColor;

struct DirectionalLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
    vec3 Direction;
};

struct Material
{
    vec3 AmbientColor;
    vec3 DiffuseColor;
};

uniform DirectionalLight gDirectionalLight;
uniform Material gMaterial;
uniform sampler2D gSampler;

void main()
{
    vec4 AmbientColor = vec4(gDirectionalLight.Color, 1.0f) *
                        gDirectionalLight.AmbientIntensity *
                        vec4(gMaterial.AmbientColor, 1.0f);

    float DiffuseFactor = dot(normalize(Normal0), -gDirectionalLight.Direction);

    vec4 DiffuseColor = vec4(0, 0, 0, 0);

    if (DiffuseFactor > 0) {
        DiffuseColor = vec4(gDirectionalLight.Color, 1.0f) *
                       gDirectionalLight.DiffuseIntensity *
                       vec4(gMaterial.DiffuseColor, 1.0f) *
                       DiffuseFactor;
    }

    FragColor = texture(gSampler, TexCoord0.xy) *
                (AmbientColor + DiffuseColor);
}
)GLSL";


GLuint loadShaders(const char vertexPath[], const char fragmentPath[]) {
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertexPath, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertexPath);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragmentPath, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertexPath);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragmentPath);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
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

/*
	standardized main to catch errors.
	In this simplified version each error is just reported as a string
	It would be better to also track which file and line number the error
	happened in, but that would take an exception object.
	For now, keeping it simple
 */
int main(int argc, char* argv[]) {
	try {
		glmain();
		glfwTerminate();		// Close OpenGL window and terminate GLFW
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