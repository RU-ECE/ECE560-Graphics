#pragma once

/*
    Unified graphics core header supporting both OpenGL and Vulkan
    Common interface with API-specific implementations

    The programmer's only responsibility is to create app = new OpenGLApp(); or app = new VulkanApp();
    This dramatically reduces the size and complexity of demo code.

    Dov Kruger
    2025-03-11
*/

#include <iostream>
#include <cstdint>
#include <string>
#include <assert.h>
#include <vector>
#include "ex.hh"

// Common color class used by both APIs
class color {
public:
    float r, g, b;
    color(float r = 0, float g = 0, float b = 0) : r(r), g(g), b(b) {}
    friend std::ostream& operator<<(std::ostream& os, color c) {
        return os << "(r=" << c.r << ", g=" << c.g << ", b=" << c.b << ")";
    }
};

// Common application settings
struct app_settings {
    std::string title;
    uint32_t width;
    uint32_t height;
    color bg_color;
#if VULKAN
    VkApplicationInfo app_info;
#endif
};

/**
 * Common base class for both OpenGL and Vulkan applications
 * By defining many virtual functions, with standardized names we can include them in any code that
 * needs a standard set of controls without having to write them over and over again.
 *  */

class app_base {
public:
    virtual ~app_base();
    virtual void init() = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0;

    virtual void pan_left();
    virtual void pan_right();
    virtual void pan_up();
    virtual void pan_down();
    virtual void zoom_in();
    virtual void zoom_out();

    virtual void rotate_left();
    virtual void rotate_right();
    virtual void rotate_up();
    virtual void rotate_down();
    virtual void rotate_clockwise();
    virtual void rotate_counterclockwise();

    virtual void scale_up();
    virtual void scale_down();

    virtual void toggle_fullscreen();
    virtual void toggle_wireframe();
    virtual void toggle_culling();
    virtual void toggle_depth_testing();
    virtual void toggle_blending();
    virtual void toggle_face_culling();
    virtual void toggle_depth_buffering();
    virtual void toggle_stencil_buffering();
    virtual void toggle_msaa();
    virtual void toggle_v_sync();
    virtual void toggle_gamma_correction();
};

// API-specific includes and definitions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#if OPENGL
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#elif VULKAN
    #define GLFW_INCLUDE_VULKAN
    #include <GLFW/glfw3.h>
    #include "vulkan/vulkan.h"

    #ifdef _WIN64
        #define VK_USE_PLATFORM_WIN32_KHR
        #include "vulkan/vk_platform.h"
    #else
        #define VK_USE_PLATFORM_XCB_KHR
    #endif

#endif

void std_3d_camera_controls(); // controls moving moving around a 3d world
void std_2d_camera_controls(); // controls moving a 2d plane in ortho projection
void std_document_controls();  // controls for moving around a document-type interface

#if OPENGL
void dump(glm::mat4& mat);
void transpt(glm::mat4& m, double x, double y, double z);
void transform_dump(glm::mat4& mat, double x, double y, double z);
GLuint loadWebPTexture(const char* filePath);
GLuint build_prog(const char vertex_shader[], const char fragment_shader[]);
uint32_t make_static_vbo(const float vert[], uint32_t len);
const uint32_t INVALID_UNIFORM_LOCATION = 0xFFFFFFFF;

// convenience function for creating a vao and vbo from an array of floats
inline void make_vao_vbo(uint32_t& vao, uint32_t& vbo, const float vert[], uint32_t len) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, len * sizeof(float), vert, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// convenience function for binding vao and vbo with just x,y points
void bindxy(uint32_t vao, uint32_t vbo);

// convenience function for binding vao and vbo with x,y,r,g,b points
void bindxyrgb(uint32_t vao, uint32_t vbo);

// convenience function for binding vao and vbo with x,y,z,r,g,b points
void bindxyzrgb(uint32_t vao, uint32_t vbo);

// convenience function for binding vao and vbo with x,y,z,u,v for textures
void bindxyzuv(uint32_t vao, uint32_t vbo);

// convenience function for binding vao and vbo with x,y,z,value
//for a heat map looking up the color in a 1D texture
void bindxyz_val(uint32_t vao, uint32_t vbo);

// convenience function for unbinding vao and vbo
void unbind(uint32_t vao, uint32_t vbo);
#endif

extern app_settings settings;
extern app_base* app;
