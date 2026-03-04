#include <iostream>
#include "common/common.hh"
#include <vector>
#include "Shape.hpp"

using namespace std;

float RAD2DEG(float x) { return x * (180 / M_PI); }

// sizeof(point3d) should be 6 floats*4 bytes per float = 24 bytes	
vector<point3d> create_cylinder(int resolution) {
	// create a cylinder with the given resolution (number of points around the circle)
	// return an array of vertices, each vertex is x,y,r,g,b
	// THERE IS NO TOP OR BOTTOM, just the sides
	vector<point3d> vertices;
	for (int i = 0; i <= resolution; i++) {
		float angle =  (2 * M_PI/resolution) * i;
		float x = cos(angle);
		float z = sin(angle);
		cout << "vertex " << i << " is at " << x << ",0," << z << endl;
		vertices.push_back({x, 0, z, 1, 1, 1}); // white color
		vertices.push_back({x, 1, z, 1, 0, 0}); // red color
	}
	return vertices;
}

void glmain() {
	win = createWindow(1000, 800, "lighting demo");

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);	// Dark blue background
	uint32_t programID = loadShaders( "07lighting.vert", "07lighting.frag" );

	uint32_t vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	static vector<point3d> vert;
	static vector<uint32_t> indices;
	static vector<float> values;
	const int rows = 5, cols = 4;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
		values.push_back(j / float(cols-1));
	}

	Shape::heatmap(rows, cols, vert, indices, values);

	uint32_t vbo;
	uint32_t vbo_length = vert.size();
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point3d) * vert.size(), &vert[0], GL_STATIC_DRAW);
	glBindVertexArray(0); // we aren't working with vao any more
	uint32_t ebo; // element buffer object
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

	float a = 0;
	do {
		glClear( GL_COLOR_BUFFER_BIT );  	// Clear the screen
		glUseProgram(programID);      		// Use our shader
	  	glBindVertexArray(vao);           // draw using vao and its vbo, and anything else inside it

		// 1st attribute buffer : vertices (x,y,z)
		// 2nd attribute buffer : colors (r,g,b)

		// each point is: xyz rgb
		//                0   1
		glVertexAttribPointer(
			0,                  // first parameter to shader, numbered 0
			3,                  // 3 floating point numbers (x,y,z)
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			6*sizeof(float),    // there are 6 numbers total, this uses first 3
			(void*)0            // array buffer offset
		);
		glVertexAttribPointer(
			1,                     // 2nd parameter to shader, numbered 1
			3,                     // 3 floating point numbers r,g,b
			GL_FLOAT,              // all these values are float
			GL_FALSE,              // normalized?
			6*sizeof(float),       // there are 5 numbers total, this uses first 2
			(void*)(3*sizeof(float)) // after x,y,z offset of rgb = 3*sizeof(float)
		);

		glEnableVertexAttribArray(0); // pass x,y,z to shader
		glEnableVertexAttribArray(1); // pass rgb to shader

		glm::mat4 transform = glm::identity<glm::mat4>(); // default should be identity matrix
//		transform = glm::scale(transform, glm::vec3(2,0.5,1));
		transform = glm::rotate(transform, RAD2DEG(a), glm::vec3(1,0,0)); // rotate around y axis (up)
//		transform = glm::translate(transform, glm::vec3(1.0,0,0));
		uint32_t matrixID = glGetUniformLocation(programID, "transform");
//		dump(transform);
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &transform[0][0]);
//#endif
		
		// Draw the shape
//		glDrawArrays(GL_TRIANGLE_STRIP, 0, vbo_length); // the numnber of indices to draw

		// start from the beginning and draw the whole thing
		glDrawElements(GL_TRIANGLE_STRIP, vbo_length, GL_UNSIGNED_INT, (void*)(0*sizeof(uint32_t)));

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

