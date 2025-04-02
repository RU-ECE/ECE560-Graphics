# Builds the following executables in bin/
# Directory structure:
#
# bin/ executable directory, ignored in github. All executables are built here.
# src/ source code directory
# src/vulkan/ vulkan source code directory
# src/opengl/ opengl source code directory
# include/ common header files directory
#
# bin/
# 01_window: bare window with only opengl clear screen to background color
# 02_triangle: draw a triangle using a fixed color shader
# 03_triangleonecolor: draw a triangle passing the color as a uniform
# 04_gouraud: draw a triangle using a gouraud shader xyrgb
# 05_rect: draw a rectangle using GL_TRIANGLE_STRIP
# 06_transform: draw a gouraud-shaded triangle and transform it with a view matrix
# 07_projection: fly around 2 triangles using a projection shader
# 08_projection_panright: move the camera right so the object moves left
# 09_sphere: draw a wireframe sphere
# 10_sphere: draw a textured sphere
# 11_rectangle_lighting: draw a rectangle with lighting
# 12_convert_obj: convert obj to blockloader format
# 13_load_obj: load obj file just to benchmark load times
# 14_draw_blobj: draw blockloader object at warp speed
# TODO: idea add gridmesh to blockloader so it generates the indices instea
#       of storing them.
# TODO: map inputs into a single unified hashmap binding system
# TODO: render text
# TODO: draw dots (show a selected object with 4 dots around it?)
# TODO: Draw two views side by side from different camera angles
# TODO: use GuiImage to create user interface

cmake_minimum_required(VERSION 3.10)
project(demos)

# Add near the top of the file, after project()
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Add debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Define OPENGL for the graphics core header
add_definitions(-DOPENGL=1)

# Set OpenGL preference to GLVND before finding packages
set(OpenGL_GL_PREFERENCE GLVND)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set(CMAKE_CXX_STANDARD 14)

# Find required packages
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Add the common directory to the include path
include_directories(../common)  # the utility code shared by OpenGL, raytracing and Vulkan as well as the libraries for each
# includes exceptions, loading/saving images, video, and opengl.core.cpp and vulkan.core.cpp, the plug compatible core code for each

# Add path to find the library
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common/build)

# Function to add OpenGL executable with standard linking
function(add_opengl_executable TARGET_NAME SOURCE_FILE)
    add_executable(${TARGET_NAME} ${SOURCE_FILE})
    target_link_libraries(${TARGET_NAME} PRIVATE
        opengl_core
        OpenGL::GL
        ${GLEW_LIBRARIES}
        glfw
        webp
    )
endfunction()

add_opengl_executable(01_window "01_window.cpp")
add_opengl_executable(02_triangle "02_triangle.cpp")
add_opengl_executable(03_trianglecolor "03_trianglecolor.cpp")
add_opengl_executable(04_gouraud "04_gouraud.cpp")
add_opengl_executable(05_rect "05_rect.cpp")
add_opengl_executable(06_transform "06_transform.cpp")
add_opengl_executable(08_keymap "08_keymap.cpp")
#add_opengl_executable(06_projection "06_projection.cpp")
#add_opengl_executable(07_sphere "07_sphere.cpp")
#add_opengl_executable(08_rectangle_lighting "08_rectangle_lighting.cpp")
#add_opengl_executable(09_convert_obj "09_convert_obj.cpp")
#add_opengl_executable(10_load_obj "10_load_obj.cpp")
#add_opengl_executable(10_draw_blobj "10_draw_blobj.cpp")


# Vulkan section (currently commented out)
#set(VULKAN_SRC src/vulkan)
#include_directories(${VULKAN_SRC})
#remove_definitions(OPENGL)
#add_compile_definitions(VULKAN=1)

#add_library(vulkan_core STATIC ${VULKAN_SRC}/vulkan_core.cpp)

#add_executable(01v_window ${VULKAN_SRC}/01_window.cpp)
#target_link_libraries(01v_window vulkan_core)

#add_executable(02v_triangle ${VULKAN_SRC}/02_triangle.cpp)
#target_link_libraries(02v_triangle vulkan_core)

#add_executable(02vb_rect ${VULKAN_SRC}/02b_rect.cpp)
#target_link_libraries(02vb_rect vulkan_core)

#add_executable(03v_gouraud ${VULKAN_SRC}/03_gouraud.cpp)
#target_link_libraries(03v_gouraud vulkan_core)

#add_executable(04v_transform ${VULKAN_SRC}/04_transform.cpp)
#target_link_libraries(04v_transform vulkan_core)

#add_executable(05v_projection ${VULKAN_SRC}/05_projection.cpp)
#target_link_libraries(05v_projection vulkan_core)

#add_executable(06v_sphere ${VULKAN_SRC}/06_sphere.cpp)
#target_link_libraries(06v_sphere vulkan_core)

#add_executable(07v_sphere ${VULKAN_SRC}/07_sphere.cpp)
#target_link_libraries(07v_sphere vulkan_core)

#add_executable(08v_rectangle_lighting ${VULKAN_SRC}/08_rectangle_lighting.cpp)
#target_link_libraries(08v_rectangle_lighting vulkan_core)

#add_executable(09v_convert_obj ${VULKAN_SRC}/09_convert_obj.cpp)
#target_link_libraries(09v_convert_obj vulkan_core)

#add_executable(10v_load_obj ${VULKAN_SRC}/10_load_obj.cpp)
#target_link_libraries(10v_load_obj vulkan_core)

#add_executable(10v_draw_blobj ${VULKAN_SRC}/10_draw_blobj.cpp)
#target_link_libraries(10v_draw_blobj vulkan_core)
