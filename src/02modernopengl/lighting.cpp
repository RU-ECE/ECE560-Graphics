#include "util.hh"
#include "shaders_lighting.hh"
using namespace std;
/*
	standardized main to catch errors.
	In this simplified version each error is just reported as a string
	It would be better to also track which file and line number the error
	happened in, but that would take an exception object.
	For now, keeping it simple
 */
void glmain(GLFWwindow* win) {
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	uint32_t programID = build_prog(vs_lighting, fs_lighting);
	uint32_t vao = make_vao();
	uint32_t vbo = make_static_vbo(vertices, sizeof(vertices));
	camera_3d cam(glm::vec3(0,0,5), glm::vec3(0,0,0), glm::vec3(0,1,0), 45.0f);
	glm::mat4 t = glm::identity<glm::mat4>();
	do {
		glClear( GL_COLOR_BUFFER_BIT );  	// Clear the screen
		glUseProgram(programID);      		// Use our shader
		bindxyrgb(vao);
		uint32_t matrixID = glGetUniformLocation(programID, "transform");
		cam.view(matrixID, t);
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
		unbind();
		glfwSwapBuffers(win); // double buffer
		glfwPollEvents();
	}	while( glfwGetKey(win, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
			   glfwWindowShouldClose(win) == 0 );
		cleanup(vao, vbo, programID);
	return 0;
}
