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
class FunctionViewer {
 private:
	uint32_t progid; // handle to the shader code
	uint32_t vao; // array object container for vbo and indices
	uint32_t vbo; // handle to the point data on the graphics card
  uint32_t lbo; // handle to buffer of indices for lines for wireframe
  uint32_t sbo; // handle to buffer of indices for triangles (solid faces)
  float xmin, xmax, ymin, ymax;
  uint32_t indexSize;
  uint32_t xres, yres;
  vec3 minColor, maxColor;
 public:
  /**
   * @brief Construct the surface of a 3d function f(x,y)
   *
   * @param f the function
   * @param xmin smallest x value on the grid
   * @param xmax largest x value on the grid
   * @param ymin smallest y value on the grid
   * @param ymax largest y value on the grid
   * @param xres number of samples in x
   * @param xmax number of samples in y
   */
  template<typename Func>
  FunctionViewer(Func f, float xmin, float xmax, float ymin, float ymax, uint32_t xres, uint32_t yres,
   const vec3& minColor, const vec3& maxColor)
  : xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax), xres(xres), yres(yres), minColor(minColor), maxColor(maxColor) {
  progid = loadShaders( "07_heatmap.vert", "07_heatmap.frag" );  
  uint32_t count = (xres+1)*(yres+1), size = count * 3;
  
  float vert[size]; // x,y,z

  float vals[count]; // value used to convert to color (heatmap)
  uint32_t c = 0, d = 0, e = 0;
  float dx = (xmax - xmin) / xres, dy = (ymax - ymin) / yres;
  float y = ymin;
  for (uint32_t j = 0; j <= yres; j++, y += dy) {
    float x = xmin;
    for (uint32_t i = 0; i <= xres; i++, x += dx) {
      vert[c++] = x;
      vert[c++] = y;
      vert[c++] = f(x,y);
    }
  }
  size = c;
  // indices has extra yres used for separators, drawing yres separate triangle strips
  indexSize = 2* count + yres;
  uint32_t indices[indexSize]; // value used to convert to color (heatmap)
  const uint32_t separator = 0xFFFFFFFF;
  // this time, build indices for the boxes between the edges
  // so the numbers are 1 less in each dimension
  c = 0; d = 0;
  for (uint32_t j = 0; j < yres; j++) {
    for (uint32_t i = 0; i < xres; i++, d++) {
      indices[c++] = d;
      indices[c++] = d+xres;
    }
    indices[c++] = separator;
  }
  cout << "c=" << c << " sizeof indices=" << indexSize << '\n';
  indexSize = c;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), vert, GL_STATIC_DRAW);
  //glBindBuffer(GL_ARRAY_BUFFER, valbo);
  //glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), vals, GL_STATIC_DRAW);
  glGenBuffers(1, &sbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize*sizeof(float), indices, GL_STATIC_DRAW);
  glBindVertexArray(0);
  }

	~FunctionViewer() { cleanup(); }
  void render(mat4& trans);
  void cleanup();
};

void FunctionViewer::render(mat4& trans) {
	glUseProgram(progid);      		// Use the shader
	uint32_t matrixID = glGetUniformLocation(progid, "trans");
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &trans[0][0]);

  uint32_t minColorID = glGetUniformLocation(progid, "minColor");
  glUniform3f(minColorID, minColor.r, minColor.g, minColor.b);
  uint32_t maxColorID = glGetUniformLocation(progid, "maxColor");
  glUniform3f(maxColorID, maxColor.r, maxColor.g, maxColor.b);
  uint32_t minValID = glGetUniformLocation(progid, "minVal");
  glUniform1f(minValID, -3.0f);
  uint32_t maxValID = glGetUniformLocation(progid, "maxVal");
  glUniform1f(maxValID, 3.0f);
	

  glBindVertexArray(vao);
	glVertexAttribPointer(
   	0,                  // first parameter to shader, numbered 0
		3,                  // 3 floating point numbers (x,y,z)
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // this is the entire set of data, move on
	  (void*)0            // array buffer offset
	);
  glEnableVertexAttribArray(0); // pass x,y,z to shader, z doubles as the color value

  dump(trans);
  transpt(trans, -20,-20,0);
  transpt(trans, +20,-20,0);
  transpt(trans, 0,0,0);
  transpt(trans, 1,1,3);

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(0xFFFFFFFFU);
  glDrawElements(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT, (void*)0);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
  glDisable(GL_PRIMITIVE_RESTART);
}

void FunctionViewer::cleanup() {
	glDeleteBuffers(1, &vbo);	// remove vbo memory from graphics card
	glDeleteVertexArrays(1, &vao); // remove vao from graphics card
	glDeleteProgram(progid);
}

using namespace std;

double f(double x, double y) {
  double r = sqrt(x*x + y*y); // distance from (0,0)
  return 3*sin(r)*exp(-.06*r);
}

double g(double x, double y) {
  double r = sqrt(x*x + y*y);
  return 3*cos(r)*exp(-.04*r);
}

void glmain() {
	win = createWindow(800, 800, "FunctionViewer demo");

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);	// Dark blue background
	FunctionViewer viewer(g, -20, +20, -20, +20, 100, 100, vec3(0,0,1), vec3(1,0,0));

// move between two points, viewing the function as the view changes
  vec3 eye1(1000,40,150); // location of viewpoint at start
  vec3 eye2(-100,0,20); // location of viewpoint at end
  float f = 0;
  float z = 40;
  float t = 0;
  const float r = 40;
	do {
    vec3 eye = eye1 * (1-f) + eye2 * f; // linearly interpolate
    //vec3 eye(r * cos(t), r * sin(t), z);
    mat4	trans= lookAt(vec3(0,0,0), eye, vec3(0,0,1));
    trans = scale(trans, vec3(0.1,0.1,.1));
		glClear( GL_COLOR_BUFFER_BIT );  	// Clear the screen
		glDisable(GL_DEPTH_TEST);
		viewer.render(trans);
		glfwSwapBuffers(win);
		glfwPollEvents();
    f += 0.00125;
    if (f >= 1)
      f = 0;
    z -= 0.01;
    if (z <= 5)
      z = 40;
    t += .01;
	}	while( glfwGetKey(win, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
					 glfwWindowShouldClose(win) == 0 );
}
