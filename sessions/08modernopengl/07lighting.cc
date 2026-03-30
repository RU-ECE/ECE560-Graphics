#include <iostream>
#include "common/common.hh"
#include <vector>
#include "Shape.hpp"

using namespace std;

float RAD2DEG(float x) { return x * (180 / M_PI); }

struct ptmat {
    float x, y, z; // location of vertex
    float nx, ny, nz; // normal vector at vertex
};

// sizeof(point3d) should be 6 floats*4 bytes per float = 24 bytes	
vector<ptmat> create_cylinder(int resolution) {
	// create a cylinder with the given resolution (number of points around the circle)
	// return an array of vertices, each vertex is x,y,z and nx,ny,nz
	// THERE IS NO TOP OR BOTTOM, just the sides
	vector<ptmat> vertices;
	for (int i = 0; i <= resolution; i++) {
		float angle =  (2 * M_PI/resolution) * i;
		float x = cos(angle);
		float z = sin(angle);
		cout << "vertex " << i << " is at " << x << ",0," << z << endl;
        float nx = x, ny = 0, nz = z; // this IS A UNIT VECTOR because the cylinder is r=1
		vertices.push_back({x, 0, z, nx, ny, nz}); // store normals at each point
		vertices.push_back({x, 1, z, nx, ny, nz}); 
        // note: we never did anything about top and bottom
        // what would we do about the corners anyway? 
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

    vector<ptmat> vertices = create_cylinder(6);

	uint32_t vbo;
	uint32_t vbo_length = vert.size();
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point3d) * vert.size(), &vert[0], GL_STATIC_DRAW);
	glBindVertexArray(0); // we aren't working with vao any more

	float a = 0;
    glm::vec3 lightDir = glm::normalize(glm::vec3(1, 1, 1));
    glm::vec3 lightColor = glm::vec3(1, 1, 1); // white

    uint32_t lightDirID = glGetUniformLocation(programID, "lightDir");
    glUniform3fv(lightDirID, 1, glm::value_ptr(lightDir));
    glm::vec3 objectColor = glm::vec3(1, 1, 1); 
    uint32_t objectColorID = glGetUniformLocation(programID, "objectColor");
    glUniform3fv(objectColorID, 1, glm::value_ptr(objectColor));
    glm::vec3 viewPos = glm::vec3(0, 0, 5);  // eye in front of scene (looking down -Z)
    uint32_t viewPosID = glGetUniformLocation(programID, "viewPos");
    glUniform3fv(viewPosID, 1, glm::value_ptr(viewPos));
    
    glm::vec3 materialAmbient = glm::vec3(0.1, 0.1, 0.1); // dark gray
    uint32_t materialAmbientID = glGetUniformLocation(programID, "materialAmbient");
    glUniform3fv(materialAmbientID, 1, glm::value_ptr(materialAmbient));
    glm::vec3 materialDiffuse = glm::vec3(0.5, 0.5, 0.5);
    uint32_t materialDiffuseID = glGetUniformLocation(programID, "materialDiffuse");
    glUniform3fv(materialDiffuseID, 1, glm::value_ptr(materialDiffuse));
    glm::vec3 materialSpecular = glm::vec3(1, 0, 0);
    uint32_t materialSpecularID = glGetUniformLocation(programID, "materialSpecular");
    glUniform3fv(materialSpecularID, 1, glm::value_ptr(materialSpecular));
    float materialShininess = 32.0f;
    uint32_t materialShininessID = glGetUniformLocation(programID, "materialShininess");
    glUniform1f(materialShininessID, materialShininess);
    
	
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
			3,                     // 3 floating point numbers nx,ny,nz
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
		glDrawArrays(GL_TRIANGLE_STRIP, 0, vbo_length);

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

