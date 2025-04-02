cmake_minimum_required(VERSION 3.10)
project(common)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_library(opengl_core STATIC opengl_core.cpp)

target_link_libraries(opengl_core PUBLIC
    OpenGL::GL
    ${GLEW_LIBRARIES}
    glfw
    webp
)

# Make headers in this directory available to all targets that link against opengl_core
target_include_directories(opengl_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add utility executables (no OpenGL dependency)
add_executable(convert_obj_blockloader convert_obj_blockloader.cpp)
target_link_libraries(convert_obj_blockloader PRIVATE assimp)

add_executable(loadmodel loadmodel.cpp)
target_link_libraries(loadmodel PRIVATE opengl_core)
