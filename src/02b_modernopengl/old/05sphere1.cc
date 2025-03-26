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
class Sphere {
 private:
	uint32_t progid; // handle to the shader code
	uint32_t vao; // array object container for vbo and indices
	uint32_t vbo; // handle to the point data on the graphics card
  uint32_t lbo; // handle to buffer of indices for lines for wireframe sphere
  uint32_t latRes, lonRes;
  uint32_t resolution;
 public:
  /**
   * @brief Construct a sphere
   *
   * @param r radius of the sphere
   * @param latRes resolution of the grid in latitude
   * @param lonRes resolution of the grid in latitude
   */
  Sphere(double r, uint32_t latRes, uint32_t lonRes);
	~Sphere() { cleanup(); }
  void render(mat4& trans);
  void cleanup();
};


Sphere::Sphere(double r, uint32_t latRes, uint32_t lonRes) : latRes(latRes), lonRes(lonRes),
   resolution((2*latRes-2)*lonRes + 2) {
	progid = loadShaders( "05_3d.vert", "02simple.frag" );
  double dlon = 2.0*numbers::pi / lonRes, dlat = numbers::pi / latRes;
  double z;
  double lat = -numbers::pi/2 + dlat; // latitude in radians
  double rcircle;
  float vert[resolution*3]; // x,y,z
  uint32_t c = 0;
  for (uint32_t j = 0; j < 2*latRes-2; j++, lat += dlat) {
    //what is the radius of hte circle at that height?
    rcircle = r* cos(lat); // size of the circle at this latitude
    z = r * sin(lat); // height of each circle
    double t = 0;
    for (uint32_t i = 0; i < lonRes; i++, t += dlon) {
      vert[c++] = rcircle * cos(t), vert[c++] = rcircle * sin(t);
      vert[c++] = z;
    }
    cout << endl;
  }
  // south pole
  vert[c++] = 0;
  vert[c++] = 0;
  vert[c++] = -r;
  
  // north pole
  vert[c++] = 0;
  vert[c++] = 0;
  vert[c++] = r;

  cout << "resolution: " << resolution << endl;
  cout << "predicted num vert components: " << resolution*3 << endl;  
  cout << "actual num vert components: " << c << endl;

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, resolution*3*sizeof(float), vert, GL_STATIC_DRAW);
  glBindVertexArray(0);
}

void Sphere::render(mat4& trans) {
	glUseProgram(progid);      		// Use the shader
	uint32_t matrixID = glGetUniformLocation(progid, "trans");
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &trans[0][0]);

  glBindVertexArray(vao);
	glVertexAttribPointer(
   	0,                  // first parameter to shader, numbered 0
		3,                  // 3 floating point numbers (x,y,z)
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // this is the entire set of data, move on
	  (void*)0            // array buffer offset
	);
  glEnableVertexAttribArray(0); // pass x,y to shader

  glEnable(GL_PROGRAM_POINT_SIZE);

	//points don't work, why not? And how to set the size of the points?
	glPointSize(5);
  glDrawArrays(GL_POINT, 0, resolution);

	// line strips work, but incomplete (see screen shot)
	glDrawArrays(GL_POINTS, 0, resolution);
  glDisableVertexAttribArray(0);
}

void Sphere::cleanup() {
	glDeleteBuffers(1, &vbo);	// remove vbo memory from graphics card
	glDeleteVertexArrays(1, &vao); // remove vao from graphics card
	glDeleteProgram(progid);
}

using namespace std;

void glmain() {
	win = createWindow(800, 800, "Sphere demo");

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);	// Dark blue background
	Sphere sphere(1.0, 30, 15);
	mat4	trans= lookAt(vec3(0,0,0), vec3(10,5,10), vec3(0,1,0));
	do {
		glClear( GL_COLOR_BUFFER_BIT );  	// Clear the screen
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);
		sphere.render(trans);
		glfwSwapBuffers(win);             // double buffer
		glfwPollEvents();
	}	while( glfwGetKey(win, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
					 glfwWindowShouldClose(win) == 0 );
}
