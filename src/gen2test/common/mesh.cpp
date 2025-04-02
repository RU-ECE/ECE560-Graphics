#include "mesh.hh"
#include <GL/glew.h>

/*
    Implementation of the graphics methods for the mesh class.
    This file can be included into OpenGL.core.cpp to allow OpenGL programs to load 3d meshes very fast.

    Author: Dov Kruger
    Date: 2025-04-02

*/
/**
 * Initialize the mesh for graphics purposes.
 */
void mesh::init() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(sizeof(buffers)/sizeof(uint32_t), buffers);
    
    // Use the parasitic vector's data pointer and size
    glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3f), &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(vec3i), &indices[0], GL_STATIC_DRAW);
}


/**
 * Render all objects in the mesh
 * Right now, just as lines.
 * TODO: add many different rendering methods
 */
void mesh::render() {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTEX_BUFFER]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX_BUFFER]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // wireframe mode
    for (unsigned int i = 0; i < meshes.size(); i++) {
        mesh_entry& me = meshes[i];
        glDrawElements(GL_TRIANGLES, me.num_indices * 3, GL_UNSIGNED_INT, 
                      (void*)(me.base_index * sizeof(vec3i)));
    }
}
