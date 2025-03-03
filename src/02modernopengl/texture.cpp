#include "util.hh"
#include "shaders_texture.hh"

/*
	demo with texture
 */
void glmain(GLFWwindow* win) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	uint32_t programID = build_prog(vs_texture, fs_texture);
	const float xyzuv[] = {
	-1.0f, +1.0f, 0.0,   0, 0,
	-1.0f, -1.0f, 0.0,   0, 1,
	 1.0f, +1.0f, 0.0,   1, 0,
	 1.0f, -1.0f, 0.0,   1, 1
	};
	uint32_t vao = make_vao();
	uint32_t vbo = make_static_vbo(xyzuv, sizeof(xyzuv));
    GLuint textureID = loadWebPTexture("earth.webp"); // Load the texture

	glUseProgram(programID);      		// Use our shader
	bindxyzuv(vao);
	do {
		glClear( GL_COLOR_BUFFER_BIT );  	// Clear the screen
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glUniform1i(glGetUniformLocation(programID, "textureSampler"), 0);

		glBindVertexArray(vao);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Position
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // Texture coordinates
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		glfwSwapBuffers(win); // double buffer
		glfwPollEvents();
	}	while( glfwGetKey(win, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
			   glfwWindowShouldClose(win) == 0 );
}
