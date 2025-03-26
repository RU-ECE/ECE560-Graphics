/*
    Textured Sphere demo
    Author: Dov Kruger
    Date: 2025-01-07

    Load a webp cylindrical projection of earth and map to the sphere
    Tilt earth axis to 23.5 degrees and rotate
    Allow speedup and slowdown with keystrokes
*/
#include <GL/glew.h>
#include "common/common.hh"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <numbers>
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <string>
using namespace std;
using namespace glm;
using namespace std::numbers;
constexpr double PI = numbers::pi;

class Sphere {
private:
    uint32_t progid; // handle to the shader code
    uint32_t vao; // array object container for vbo and indices
    uint32_t vbo; // handle to the point data on the graphics card
    uint32_t lbo; // handle to buffer of indices for lines for wireframe sphere
    uint32_t latRes, lonRes;
    uint32_t resolution;
    uint32_t indexSize;
public:
    /**
     * @brief Construct a sphere
     *
     * @param r radius of the sphere
     * @param latRes resolution of the grid in latitude
     * @param lonRes resolution of the grid in latitude
     * @param texturePath path to the texture image
     */
    Sphere(double r, uint32_t latRes, uint32_t lonRes);
    ~Sphere() { cleanup(); }
    void render(mat4& trans, GLuint textureID);
    void cleanup();
};

Sphere::Sphere(double r, uint32_t latRes, uint32_t lonRes) : latRes(latRes), lonRes(lonRes),
    resolution((2*latRes-1)*lonRes + 2) {
    progid = loadShaders("06b_texturepoints.vert", "06b_textures.frag");
//    progid = loadShaders("03gouraud.vert", "03gouraud.frag");
    double dlon = 2.0*PI / lonRes, dlat = PI / (2*latRes);
    double z;
    double lat = -PI/2 + dlat; // latitude in radians
    double rcircle;
    float vert[resolution*5]; // x,y,z,u,v
    uint32_t c = 0;
    for (uint32_t j = 0; j < 2*latRes-1; j++, lat += dlat) {
        //what is the radius of hte circle at that height?
        rcircle = r* cos(lat); // size of the circle at this latitude
        z = r * sin(lat); // height of each circle
    
        cout << "rcircle=" << rcircle << ", z=" << z << endl;
        double t = 0;
        for (uint32_t i = 0; i < lonRes; i++, t += dlon) {
            vert[c++] = rcircle * cos(t),
            vert[c++] = rcircle * sin(t);
                cout << '(' << vert[c-2] << ", " << vert[c-1] << ")  ";
            vert[c++] = z;
            vert[c++] = t / (2.0 * PI); // Correct u mapping
            vert[c++] = (lat + PI / 2.0) / PI; // Correct v mapping
        }
        cout << endl;
    }
    // south pole
    vert[c++] = 0;
    vert[c++] = 0;
    vert[c++] = -r;
    vert[c++] = 0.5;
    vert[c++] = 0;

    // north pole
    vert[c++] = 0;
    vert[c++] = 0;
    vert[c++] = r;
    vert[c++] = 0.5;
    vert[c++] = 1;

    cout << "resolution: " << resolution << endl;
    cout << "predicted num vert components: " << resolution*5 << endl;  
    cout << "actual num vert components: " << c << endl;

    indexSize = resolution * 2 + (2*4*latRes-1); 
    //TODO: North and South Poles aren't used
    uint32_t indices[indexSize]; // connect every point in circles or latitude and longitude
    c = 0;
    for (uint32_t j = 0; j < 2*latRes - 2; j++) {
        uint32_t startrow = j*lonRes;
        for (uint32_t i = 0; i < lonRes; i++) {
            indices[c++] = startrow + i;
            indices[c++] = startrow + lonRes + i;
        }
        indices[c++] = startrow;
        indices[c++] = startrow + lonRes;
        // Add degenerate triangles to connect strips
//        indices[c++] = (j + 1) * lonRes + (lonRes - 1);
//        indices[c++] = (j + 1) * lonRes;
        indices[c++] = (j + 1) * lonRes;
        indices[c++] = (j + 1) * lonRes;
    }
    cout << "indexSize: " << indexSize << endl;
    cout << "actual grid indices: " << c << endl;

    indexSize = c; // not sure why the computaiton is off...
    // Print index data
    cout << "Index data: " ;
    for (size_t i = 0; i < indexSize; i += 2) {
        cout << '(' << indices[i] << ", " << indices[i+1] << ") ";
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, resolution*5*sizeof(float), vert, GL_STATIC_DRAW);
    glGenBuffers(1, &lbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize*sizeof(uint32_t), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
}




void Sphere::render(mat4& trans, GLuint textureID) {
    glUseProgram(progid);      		// Use the shader
    uint32_t matrixID = glGetUniformLocation(progid, "transform");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &trans[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(progid, "textureSampler"), 0);

    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // Texture coordinates
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

//    glDrawArrays(GL_TRIANGLE_STRIP, 0, resolution);
    glDrawElements(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
}

void Sphere::cleanup() {
    glDeleteBuffers(1, &vbo);	// remove vbo memory from graphics card
    glDeleteBuffers(1, &lbo);	// remove lbo (line indices)
    glDeleteVertexArrays(1, &vao); // remove vao from graphics card
    glDeleteProgram(progid);
}

using namespace std;
using namespace numbers;

void glmain() {
    win = createWindow(800, 800, "Sphere demo");

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);	// Dark blue background
    GLuint textureID = loadWebPTexture("earth.webp"); // Load the texture
    Sphere sphere(1.0, 20, 30);
    float rotAngle = 0, dRotAngle = 0.0052;
    mat4 northup = rotate(mat4(1.0f), float(PI/2), vec3(1, 0, 0));

//    mat4 northup = mat4(1.0f);

    vec3 up(0, 1, 0);    // normal OpenGL coordinates, x positive to right, y is up (z positive out of screen)
    do {
        mat trans = rotate(northup, radians(23.5f), vec3(0, 1, 0)); // tilt axis
        trans = rotate(trans, rotAngle, vec3(0, 0, 1)); // spin on axis
        rotAngle += dRotAngle;

        glClear(GL_COLOR_BUFFER_BIT);  	// Clear the screen
        glDisable(GL_DEPTH_TEST);
        sphere.render(trans, textureID);

        glfwSwapBuffers(win);             // double buffer
        glfwPollEvents();
    } while (glfwGetKey(win, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(win) == 0);
    glDeleteTextures(1, &textureID); // Clean up the texture
}
