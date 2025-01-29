#include <iostream>
#include "common/common.hh"
using namespace std;
using namespace glm;

void glmain() {
	win = createWindow(1000, 800, "window demo");

	glClearColor(0.0f, 0.5f, 0.5f, 0.0f);	// Teal background

	do {
		glClear( GL_COLOR_BUFFER_BIT );  	// Clear the screen
		glfwPollEvents();
	}	while( glfwGetKey(win, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(win) == 0 );
}
