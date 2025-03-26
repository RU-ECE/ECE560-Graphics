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
  uint32_t indexSize;
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

  const uint32_t endSegment = 0xFFFFFFFF;
  // account for extra indices used to separate each line drawn
  indexSize = resolution * 2 + (2*latRes-2) + lonRes; 
  //TODO: North and South Poles aren't used
  uint32_t indices[indexSize]; // connect every point in circles or latitude and longitude
  c = 0;
  uint32_t d = 0;
  for (uint32_t j = 0; j < 2*latRes-2; j++) {
    for (uint32_t i = 0; i < lonRes; i++, d++) {
      indices[c++] = d;
    }
    indices[c++] = endSegment; // end each circle of latitude with the special designated separate index (all 1 bits)
  }
  for (uint32_t i = 0; i < lonRes; i++) {
    for (uint32_t j = 0; j < 2*latRes-2; j++, d += lonRes) {
      indices[c++] = d; // connect each point to the one north of it
    }
    indices[c++] = endSegment; // end each circle of longitude with the special designated separate index (all 1 bits)
  }
  cout << "predicted grid points: " << indexSize << endl;
  cout << "actual grid indices: " << c << endl;

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, resolution*3*sizeof(float), vert, GL_STATIC_DRAW);
  glGenBuffers(1, &lbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize*sizeof(uint32_t), indices, GL_STATIC_DRAW);
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
	//glPointSize(5);
  //glDrawArrays(GL_POINT, 0, resolution);

	glDrawArrays(GL_LINE_STRIP, 0, resolution);
  #if 0
  // full grid is not working yet
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(0xFFFFFFFFU);
  glDrawElements(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT, (void*)0);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
  glDisable(GL_PRIMITIVE_RESTART);
  #endif
}

void Sphere::cleanup() {
	glDeleteBuffers(1, &vbo);	// remove vbo memory from graphics card
	glDeleteBuffers(1, &lbo);	// remove lbo (line indices)
	glDeleteVertexArrays(1, &vao); // remove vao from graphics card
	glDeleteProgram(progid);
}

using namespace std;

void glmain() {
	win = createWindow(800, 800, "Sphere demo");

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);	// Dark blue background
	Sphere sphere(1.0, 30, 15);
  float smallestRadiusOrbit = 40; // initial distance from which to look
  float largestRadiusOrbit = 400;  // largest distance
  float r = 100; // start closer in, spiral out, then get closer
  float theta = 0; // angle at which to orbit the sphere
	vec3 center(0,0,0); // sphere is at (0,0,0) so look towards its center
  vec3 up(0,1,0); // normal OpenGL coordinates, x positive to right, y is up (z positive out of screen)
  float dt = 0.0125, dz = 1, dr = 1.02;
  float z = 0;


//transformations still not working 100% right but it's interesting!
  int phaseInCount = 0;
  constexpr double pi_2 = numbers::pi/2;
  float end = pi_2;
  float orbitalAngle = 0, dA = 0.0052; // orbiting sphere at a different timescale
  do {
    float x = r * cos(theta), y = r * sin(theta);
    mat4	trans= lookAt(center, vec3(x,y,z), up); // lookat seems not to show distance at all?
    trans = scale(trans, vec3(100/r,100/r,100/r)); // shrink the objects by radius, making it look more distant
  	glClear( GL_COLOR_BUFFER_BIT );  	// Clear the screen
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);
		sphere.render(trans);
    trans = rotate(trans, orbitalAngle, vec3(0,1,0));
    orbitalAngle += dA;
    trans = translate(trans, vec3(2.0,0,0)); // distance of orbiting moon is 1 diameters of the body (-1..1?)
    trans = scale(trans, vec3(0.5,0.5,0.5)); // orbiting sphere moon is smaller
		sphere.render(trans);
		glfwSwapBuffers(win);             // double buffer
		glfwPollEvents();
    theta += dt;
    if (theta > end) { // after 1/4 revolution, start changing parameters so it gets interesting
      r *= dr; // increase size of orbit for a while
      phaseInCount++;
      if (phaseInCount > 60) { // after 60 frames transitioning, settle on orbit with new r and z 
          end = theta + pi_2; // set next limit
        if (r > largestRadiusOrbit) { // reverse direction
          dr = 1/dr; // start decreasing instead of increasing, and go twice as fast?
        } else if (r < smallestRadiusOrbit) {
          dr = 1/dr; // start decreasing instead of increasing, slow down again
        }
        if (theta > 2*numbers::pi)
          theta -= 2*numbers::pi;
        phaseInCount = 0; // reset for next time
      }
    }
//    cout << "r=" << r << '\n';
	}	while( glfwGetKey(win, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
					 glfwWindowShouldClose(win) == 0 );
}
