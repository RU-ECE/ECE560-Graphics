#include "graphics_core.hh"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
// Structure to hold shader binary info
struct ShaderBinaryInfo {
    GLenum format;
    std::vector<uint8_t> binary;
};

// Structure to store program binary info
struct ProgramBinaryInfo {
    GLenum format;
    std::vector<uint8_t> binary;
};

class BinaryShaderLoader {
private:
    std::string cache_path;
    std::string gpu_id;
    
    // Get unique GPU identifier (vendor + renderer)
    std::string get_gpu_identifier() {
        const char* vendor = (const char*)glGetString(GL_VENDOR);
        const char* renderer = (const char*)glGetString(GL_RENDERER);
        return std::string(vendor) + "_" + renderer;
    }

    // Save binary to file
    void save_binary(const std::string& filename, const void* data, size_t size) {
        std::ofstream file(filename, std::ios::binary);
        file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
        file.write(reinterpret_cast<const char*>(data), size);
    }

    // Load binary from file
    std::vector<uint8_t> load_binary(const std::string& filename, size_t& size) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) return std::vector<uint8_t>();
        
        file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        std::vector<uint8_t> data(size);
        file.read(reinterpret_cast<char*>(data.data()), size);
        return data;
    }

public:
    BinaryShaderLoader(const std::string& cache_dir = "shader_cache/") 
        : cache_path(cache_dir) {
        gpu_id = get_gpu_identifier();
    }

    GLuint load_program(const char* vs_source, const char* fs_source, const std::string& name) {
        std::string binary_path = cache_path + name + "_" + gpu_id + ".bin";
        GLuint program = 0;

        // Try to load cached binary program
        std::ifstream test(binary_path, std::ios::binary);
        if (test.good()) {
            size_t size;
            std::vector<uint8_t> binary = load_binary(binary_path, size);
            if (!binary.empty()) {
                program = glCreateProgram();
                GLint formats;
                glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
                if (formats > 0) {
                    glProgramBinary(program, GL_PROGRAM_BINARY_FORMAT_MESA, // Format may vary
                                  binary.data(), binary.size());
                    
                    GLint status;
                    glGetProgramiv(program, GL_LINK_STATUS, &status);
                    if (status == GL_TRUE) {
                        return program;
                    }
                    glDeleteProgram(program);
                }
            }
        }

        // Fallback to compilation
        program = build_prog(vs_source, fs_source);

        // Save binary for next time
        GLint length;
        glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &length);
        if (length > 0) {
            std::vector<uint8_t> binary(length);
            GLenum format;
            GLsizei binary_length;
            glGetProgramBinary(program, length, &binary_length, &format, binary.data());
            
            // Save format and binary
            save_binary(binary_path, binary.data(), binary_length);
        }

        return program;
    }
};

// Example usage:
int main() {
    // ... OpenGL initialization ...

    BinaryShaderLoader loader("shader_cache/");
    
    const char* vs_source = R"(
        #version 330
        layout(location = 0) in vec3 position;
        void main() {
            gl_Position = vec4(position, 1.0);
        }
    )";

    const char* fs_source = R"(
        #version 330
        out vec4 color;
        void main() {
            color = vec4(1.0, 0.0, 0.0, 1.0);
        }
    )";

    GLuint program = loader.load_program(vs_source, fs_source, "simple_shader");
    
    // Use program...
    glUseProgram(program);

    return 0;
}