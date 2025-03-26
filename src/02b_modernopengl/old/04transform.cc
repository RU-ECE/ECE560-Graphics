#include <iostream>
#include "common/common.hh"

using namespace std;

float RAD2DEG(float x) { return x * (180 / M_PI); }
void glmain() {
	win = createWindow(1000, 800, "Transform triangle demo");

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);	// Dark blue background

  // Create and compile our GLSL program from the shaders
	//uint32_t programID = loadShaders( "03gouraud.vert", "03gouraud.frag" );
	uint32_t programID = loadShaders( "04transform.vert", "03gouraud.frag" );

	uint32_t vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// each point is x,y, r,g,b
	static const float vertices[] = { 
		-1.0f, -1.0f,    1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,    0.0f, 1.0f, 0.0f,
		 0.0f,  1.0f,    0.0f, 0.0f, 1.0f,
	};

	uint32_t vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(0); // we aren't working with vao any more

	float a = 0;
	do {
		glClear( GL_COLOR_BUFFER_BIT );  	// Clear the screen
		glUseProgram(programID);      		// Use our shader
	  	glBindVertexArray(vao);           // draw using vao and its vbo, and anything else inside it

		// 1st attribute buffer : vertices
	//	glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glVertexAttribPointer(
			0,                  // first parameter to shader, numbered 0
			2,                  // 2 floating point numbers (x,y) z = 0
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			5*sizeof(float),    // there are 5 numbers total, this uses first 2
			(void*)0            // array buffer offset
		);
		glVertexAttribPointer(
			1,                     // 2nd parameter to shader, numbered 1
			3,                     // 3 floating point numbers r,g,b
			GL_FLOAT,              // all these values are float
			GL_FALSE,              // normalized?
			5*sizeof(float),       // there are 5 numbers total, this uses first 2
			(void*)(2*sizeof(float)) // after x,y offset of rgb = 2
		);

		glEnableVertexAttribArray(0); // pass x,y to shader
		glEnableVertexAttribArray(1); // pass rgb to shader

//#if 0
		glm::mat4 transform = glm::identity<glm::mat4>(); // default should be identity matrix
//		transform = glm::scale(transform, glm::vec3(2,0.5,1));
//		transform = glm::translate(transform, glm::vec3(0.5,0.5,0));
		transform = glm::rotate(transform, RAD2DEG(a), glm::vec3(0,1,0));
		uint32_t matrixID = glGetUniformLocation(programID, "transform");
		dump(transform);
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &transform[0][0]);
//#endif
		
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
  	glBindVertexArray(0);            // stop drawing using vao

		glfwSwapBuffers(win); // double buffer
		glfwPollEvents();
		a += .0001;
	}	while( glfwGetKey(win, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(win) == 0 );

	// Cleanup VBO
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);
}

