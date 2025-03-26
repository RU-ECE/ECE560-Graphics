#include <iostream>
#include "common/common.hh"

using namespace std;

void glmain() {
	win = createWindow(1000, 800, "Transform triangle demo");

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);	// Dark blue background

	// Create and compile our GLSL program from the shaders
	uint32_t programID = loadShaders("04c_transform.vert", "03gouraud.frag");

	uint32_t vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// each point is x,y, r,g,b
	static const float vertices[] = { 
		-.5f, -.5f,    .5f, 0.0f, 0.0f,
		 .5f, -.5f,    0.0f, 0.5f, 0.0f,
		 0.0f,  0.5f,    0.0f, 0.0f, 0.5f,
	};

	uint32_t vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(0); // we aren't working with vao any more

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	glm::vec3 eye = glm::vec3(0.0f, 0.0f, 3.0f); // Initial camera position
	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f); // Look at origin
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // Up vector

	float angle1 = 0.0f, angle2 = 0.0f;

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID); // Use our shader
		glBindVertexArray(vao);  // draw using vao and its vbo, and anything else inside it

		// Update6 the view matrix each frame
//		glm::mat4 view = glm::lookAt(eye, center, up);


		glm::mat4 view = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1000.0f, 1000.0f);
		// First triangle
		glm::mat4 model1 = glm::rotate(glm::mat4(1.0f), angle1, glm::vec3(0, 1, 0));
		glm::mat4 mvp1 = projection * view * model1;
		uint32_t matrixID = glGetUniformLocation(programID, "MVP");
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp1[0][0]);
//		cerr << "\n\n==mvp1==\n"; dump(mvp1);
        transform_dump(mvp1, 0,0,0);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Second triangle
		glm::mat4 model2 = glm::rotate(glm::mat4(1.0f), angle2, glm::vec3(0, 1, 0));
		model2 = glm::translate(model2, glm::vec3(0.5f, 0.5f, 0.0f));
		glm::mat4 mvp2 = projection * view * model2;
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp2[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glfwSwapBuffers(win);
		glfwPollEvents();

		angle1 += 0.01f;
		angle2 += 0.004f;

		// Move the camera backward
		eye.z += 0.1f;

		cout << "angle1: " << angle1 << ", angle2: " << angle2 << ", eye.z: " << eye.z << endl;
	} while (glfwGetKey(win, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(win) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);
}
