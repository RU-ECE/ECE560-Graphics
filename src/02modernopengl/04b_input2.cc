#include <iostream>
#include "common/common.hh"

using namespace std;

bool needs_update = false;
glm::vec3 eye = glm::vec3(0.0f, 0.0f, 3.0f);

// create a single uniform integer to map every potential input event
map<uint32_t, uint32_t> input_map;
map<uint32_t, void(*)()> actions;

void rotate_left() {
	a += 0.01f;
	needs_update = true;
}

void rotate_right() {
	a -= 0.01f;
	needs_update = true;
}

void zoom_in() {
	eye.z += 0.1f;
	needs_update = true;
}

void zoom_out() {
	eye.z -= 0.1f;
	needs_update = true;
}


void handle_input(uin32_t event) {
  if (input_map.find(event) == input_map.end())
    return; // input event not mapped to an action
  uint32_t action = input_map[event];
  cerr << "action: " << action << endl;
  actions[action]();
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	handle_input();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	handle_input();
}

void glmain() {
	win = createWindow(1000, 800, "Transform triangle demo");

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);	// Dark blue background

  // Create and compile our GLSL program from the shaders
	//uint32_t programID = loadShaders( "03gouraud.vert", "03gouraud.frag" );
	uint32_t programID = loadShaders("04transform.vert", "03gouraud.frag" );

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

	float a = 0;

//	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

//	glm::vec3 eye = glm::vec3(4.0f, 3.0f, 3.0f); // Camera position
//	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f); // Look at origin
//	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // Up vector

//	glm::mat4 view = glm::mat4(1.0f); //::lookAt(eye, center, up);

	glfwSetMouseButtonCallback(win, mouse_button_callback);
	glfwSetKeyCallback(win, key_callback);

	float a = 0.0f;

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glBindVertexArray(vao);

		// 1st attribute buffer : vertices
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glUseProgram(programID);	
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

		// First triangle
		glm::mat4 model1 = glm::rotate(glm::mat4(1.0f), a, glm::vec3(0, 1, 0));
		uint32_t matrixID = glGetUniformLocation(programID, "transform");
		dump(model1);
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &model1[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glfwSwapBuffers(win);
		glfwPollEvents();

		a += 0.01f;
		// Check for keyboard or mouse input
        if (glfwGetKey(win, GLFW_KEY_ANY) == GLFW_PRESS) {
            handle_input(); // Call your input handling function
        }

        // Check for any mouse button press
        if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ||
            glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS ||
            glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
            handle_input();
        }

        // Check for mouse movement
        double xpos, ypos;
        glfwGetCursorPos(win, &xpos, &ypos);
        // You can use xpos and ypos to determine if the mouse has moved
	} while (glfwGetKey(win, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(win) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);
}

