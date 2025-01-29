#include <iostream>
#include "common/common.hh"
using namespace std;
using namespace glm;

void glmain() {
	win = createWindow(1000, 800, "Gouraud triangle demo");

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);	// Dark blue background

	// Create and compile our GLSL program from the shaders
	uint32_t programID = loadShaders( "03gouraud.vert", "03gouraud.frag" );

	// each point is x,y, r,g,b
	static const float vertices[] = { 
		-1.0f, -1.0f,    1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,    0.0f, 1.0f, 0.0f,
		 0.0f,  1.0f,    0.0f, 0.0f, 1.0f,
	};

	uint32_t vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	uint32_t vbo = make_static_vbo(vertices, sizeof(vertices));

	do {
		glClear( GL_COLOR_BUFFER_BIT );  	// Clear the screen
		glUseProgram(programID);      		// Use our shader

		// 1st attribute buffer : vertices
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glVertexAttribPointer(
			0,                  // first parameter to shader, numbered 0
			2,                  // 3 floating point numbers (x,y) z = 0
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			5*sizeof(float),    // there are 5 numbers total, this uses first 2
			(void*)0            // array buffer offset
		);
		glVertexAttribPointer(
			1,                     // first parameter to shader, numbered 0
			3,                     // 3 floating point numbers (x,y) z = 0
			GL_FLOAT,              // all these values are float
			GL_FALSE,              // normalized?
			5*sizeof(float),       // there are 5 numbers total, this uses first 2
			(void*)(2*sizeof(float)) // after x,y offset of rgb = 2
		);

		glEnableVertexAttribArray(0); // pass x,y to shader
		glEnableVertexAttribArray(1); // pass rgb to shader
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glfwSwapBuffers(win); // double buffer
		glfwPollEvents();

	}	while( glfwGetKey(win, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(win) == 0 );

		glEnableVertexAttribArray(0); // pass x,y to shader
		glEnableVertexAttribArray(1); // pass rgb to shader
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glfwSwapBuffers(win); // double buffer
		glfwPollEvents();

	}	while( glfwGetKey(win, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(win) == 0 );

	// Cleanup VBO
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);
}

