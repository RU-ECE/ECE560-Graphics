#include "util.hh"
#include "shaders_lighting.hh"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "common/ogl_basic_mesh.h"
using namespace std;

class Mesh {
private:
	uint32_t vao;
	uint32_t buffers[4];
	Assimp::Importer importer;
	aiScene* scene;
	struct mesh_entry {
        mesh_entry() : num_indices(0), base_vertex(0), base_index(0), material_index(INVALID_MATERIAL)
        {}
        uint32_t num_indices;
        uint32_t base_vertex;
        uint32_t base_index;
        uint32_t material_index;
    };
public:
  Mesh() {
	glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(sizeof(buffers)/sizeof(uint32_t), buffers);
  }
  void load_mesh(const aiScene* scene, aiMesh* mesh) {
	scene = importer.ReadFile("models/antique_ceramic_vase_01_4k.obj", aiProcess_Triangulate | aiProcess_FlipUVs);
	aiMesh* mesh = scene->mMeshes[0];
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

  }
};
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
//	camera_3d cam(glm::vec3(0,0,5), glm::vec3(0,0,0), glm::vec3(0,1,0), 45.0f);
	glm::mat4 t = glm::identity<glm::mat4>();


	glUseProgram(programID);      		// Use our shader
	do {
		glClear( GL_COLOR_BUFFER_BIT );  	// Clear the screen

		glfwSwapBuffers(win); // double buffer
		glfwPollEvents();
	}	while( glfwGetKey(win, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
			   glfwWindowShouldClose(win) == 0 );
//		cleanup(vao, vbo, programID);
}
