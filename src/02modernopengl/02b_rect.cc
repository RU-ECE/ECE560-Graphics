#include <iostream>
#include "common/common.hh"
using namespace std;

void glmain() {
	win = createWindow(1000, 800, "rect demo");

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);	// Dark blue background
	// Create and compile our GLSL program from the shaders
	uint32_t programID = loadShaders( "02simple.vert", "02simple.frag" );

	static const float vertices[] = { 
		-1.0f, -1.0f, 0.0f, // bottom left
		 1.0f, -1.0f, 0.0f, // bottom right
         1.0f, 1.0, 0.0f,   // top right

         1.0f, 1.0, 0.0f,   // top right
		 -1.0, 1.0, 0.0,    // top left
		-1.0f, -1.0f, 0.0f, // bottom left
	};

	uint32_t vao; // vertex array object, a box that holds stuff to use in drawing
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	uint32_t vbo = make_static_vbo(vertices, sizeof(vertices));

	do {
		glClear( GL_COLOR_BUFFER_BIT );  	// Clear the screen
		glUseProgram(programID);      		// Use our shader

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(
			0,                  // first parameter to shader, numbered 0
			3,                  // 3 floating point numbers (x,y,z)
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride (if there is other data, need this)
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 6); // 2 points, xyz
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(win); // double buffer
		glfwPollEvents();

	}	while( glfwGetKey(win, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(win) == 0 );

	// Cleanup VBO
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);
}

