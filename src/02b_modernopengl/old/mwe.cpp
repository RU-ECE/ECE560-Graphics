#include "util.hh"
#include "shader_gouraudtransform.hh"

void glmain(GLFWwindow* win) {
	static const float vertices[] = { 	// each point is x,y, r,g,b
		-1.0f, -1.0f,    1.0f, 0.0f, 0.0f,
		1.0f, -1.0f,    0.0f, 1.0f, 0.0f,
		0.0f,  1.0f,    0.0f, 0.0f, 1.0f,
	};

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	uint32_t programID = build_prog(vs_pervertex, fs_common);
	uint32_t vao = make_vao();
	uint32_t vbo = make_static_vbo(vertices, sizeof(vertices));
	float eyex, eyey = 0, eyez;
	float viewing_radius = 5;
	float viewing_angle = 0;
	float a1 = 0;
	glUseProgram(programID);      		// Use our shader
	do {
		glClear( GL_COLOR_BUFFER_BIT );  	// Clear the screen
		bindxyrgb(vao);

		glm::mat4 t1 = glm::scale(glm::identity<glm::mat4>(), glm::vec3(0.5,0.5,1));
		glm::mat4 t2 = glm::translate(t1, glm::vec3(2,0,0));
		t1 = glm::rotate(t1, a1, glm::vec3(0,1,0));
		//t2 = glm::rotate(t2, a2, glm::vec3(0,1,0));
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		eyex = viewing_radius*cos(viewing_angle);
		eyez = viewing_radius*sin(viewing_angle);
		glm::mat4 view = glm::lookAt(glm::vec3(eyex,eyey,eyez), glm::vec3(0,0,0), glm::vec3(0,1,0));
		glm::mat4 transform =  proj * (view * t1);
		uint32_t matrixID = glGetUniformLocation(programID, "transform");
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &transform[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
		transform = proj * (view * t2);
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &transform[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
		unbind();
		glfwSwapBuffers(win); // double buffer
		a1 += .01;
		viewing_angle += .01;
		glfwPollEvents();
	}	while( glfwGetKey(win, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
				glfwWindowShouldClose(win) == 0 );
}
