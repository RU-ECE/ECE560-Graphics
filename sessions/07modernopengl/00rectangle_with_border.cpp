#include <iostream>
#include "common/common.hh"
using namespace std;
using namespace glm;

void glmain() {
	win = createWindow(1000, 800, "rectangle with border");

	glClearColor(0.0f, 0.4f, 0.0f, 0.0f);	// Dark green background
	// Create and compile our GLSL program from the shaders
	uint32_t programID = loadShaders( "00simple.vert", "00simple.frag" );

	static const float vertices[] = { 
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
	};

	uint32_t vao; // vertex array object, a box that holds stuff to use in drawing
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao); // from now on, everything is inside vao
	uint32_t vbo = make_static_vbo(vertices, sizeof(vertices)); // vertices is 36 bytes

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

		// Draw the rectangle !
        //TODO: rewrite using only 4 points using GL_TRIANGLE_STRIP
		glDrawArrays(GL_TRIANGLES, 0, 6); // draw 2 triangles (2 groups of 3 vertices each)

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

