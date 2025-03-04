#pragma once

#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/glew.h>
#include <iomanip>

using namespace std;
using namespace std::chrono;

class blockloader {
protected:
    uint8_t* data;
    size_t capacity; // allocated size of the file
    size_t size; // current size of the bytes used in the file
public:
    struct header {
        uint32_t magic;      // "BLKL" or "BLKC" for LZMA compression?
        uint16_t type;       // type of entire file (perhaps later, just have types for each block inside?)
        uint16_t version;    // version of this blockloader file
        uint64_t size;       // total size of the blockloader file aligned to 64-bit boundary
        uint64_t authorid;   // unique author id so all meta information can be stored on a server not here
        uint32_t docid;      // unique document id per author. Global unique id is authorid:docid
        uint32_t docversion; // version of the document
        uint32_t num_entries; // number of individual blocks in the file
    };
    blockloader(size_t initial_capacity, uint16_t type, uint16_t version, uint64_t authorid, uint32_t docid, uint32_t docversion)
     : data((uint8_t*)aligned_alloc(64, initial_capacity)), capacity(initial_capacity), size(sizeof(header)) {
        header* h = (header*)data;
        h->magic = 0x424c4b4c;
        h->type = type;
        h->version = version;
        h->size = size;
        h->authorid = authorid;
        h->docid = docid;
        h->docversion = docversion;
        h->num_entries = 0;
    }
    blockloader(const char* filename) {
        auto t0 = chrono::high_resolution_clock::now();
        ifstream f(filename, ios::in | ios::binary);
        f.seekg(0, ios::end);
        size = f.tellg();
        capacity = size;
        data = (uint8_t*)aligned_alloc(64, size);
        f.seekg(0, ios::beg);
        f.read(reinterpret_cast<char*>(data), size);
        header* h = (header*)data;
        size = h->size;
        auto t1 = chrono::high_resolution_clock::now();
        cout << fixed << setprecision(3) 
             << duration_cast<microseconds>(t1 - t0).count() / 1000.0 
             << "ms" << endl;
    }
    // Return a pointer to the start of the data after the header
    uint8_t* start_data() {
        return data + sizeof(header);
    }
    void check_header(uint16_t type) {
        header* h = (header*)data;
        if (h->magic != 0x424c4b4c)
            throw "invalid magic number";
        if (h->type != type)
            throw "unexpected type";
    }

    void grow(size_t new_capacity) {
        if (capacity < new_capacity) {
            const uint8_t* old = data;
            data = (uint8_t*)aligned_alloc(64, new_capacity);
            memcpy(data, old, size);
            free((void*)old);
            capacity = new_capacity;
        }
    }
    void insert(size_t offset, size_t sz, const void* src) {
        if (size + sz > capacity) {
            grow(size + sz);
        }
        memcpy(this->data + offset, src, sz);
        size += sz;
    }
    void append(const void* src, size_t sz) {
        if (size + sz > capacity) {
            grow(size + sz);
        }
        memcpy(this->data + size, src, sz);
        size += sz;  // Update the current size after appending
    }
    void save(const char filename[]);
};

struct vec3f {
    float x, y, z;
};

struct vec2f {
    float x, y;
};

struct vec3i {
    int x, y, z;
};

struct vec2i {
    int x, y;
};

struct mesh_entry {
    mesh_entry() : num_indices(0), base_vertex(0), base_index(0), material_index(-1) {}
    mesh_entry(uint32_t mat_idx, uint32_t n_indices, uint32_t b_vertex, uint32_t b_index)
        : num_indices(n_indices), base_vertex(b_vertex), base_index(b_index), material_index(mat_idx) {}
    uint32_t num_indices;
    uint32_t base_vertex;
    uint32_t base_index;
    uint32_t material_index;
};

struct material_entry {
    material_entry(uint32_t idx) : material_index(idx) {}
    uint32_t material_index;
};

template<typename T>
class parasitic_vector {
private:
    T* data;
    size_t sz;
public:
    parasitic_vector() : data(nullptr), sz(0) {}
    parasitic_vector(char*& data, size_t sz) : data((T*)data), sz(sz) {
        data += sz * sizeof(T);
    }

    T& operator[](size_t i) {
        return data[i];
    }
    const T& operator[](size_t i) const {
        return data[i];
    }
    size_t size() const {
        return sz;
    }
};

class mesh {
private:
    enum BUFFER_TYPE {
        INDEX_BUFFER = 0,
        VERTEX_BUFFER = 1,
        WVP_MAT_BUFFER = 2,  // required only for instancing
        WORLD_MAT_BUFFER = 3,  // required only for instancing
        NUM_BUFFERS = 4
    };

    struct mesh_header {
        uint32_t num_meshes;
        uint32_t num_materials;
        uint32_t num_vertices;
        uint32_t num_texcoords;
        uint32_t num_normals;
        uint32_t num_indices;
    };
    blockloader& bl;
    parasitic_vector<mesh_entry> meshes;
    parasitic_vector<material_entry> materials;
    parasitic_vector<vec3f> vertices;
    parasitic_vector<vec2f> texcoords;
    parasitic_vector<vec3f> normals;
    parasitic_vector<vec3i> indices;
	uint32_t vao;
	uint32_t buffers[6];

    void init_from_scene(aiScene* scene);
public:
  mesh(blockloader& bl, const char* filename) : bl(bl) {
    load_obj(filename, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords);
  }
  mesh(blockloader& bl);
  void init();
  void load_obj(const char filename[], int flags);
  void render();
  void save(const char filename[]);
};

void blockloader::save(const char filename[]) {
    ofstream f(filename, ios::out | ios::binary);
    f.write(reinterpret_cast<const char*>(data), size);
}

mesh::mesh(blockloader& bl) : bl(bl) {
    bl.check_header(0x0001); // TODO: register type numbers checking for mesh blockloader
    mesh_header* mh = (mesh_header*)bl.start_data();
    char* p = (char*)(mh + 1);
    meshes = parasitic_vector<mesh_entry>(p, mh->num_meshes);
    materials = parasitic_vector<material_entry>(p, mh->num_materials);
    vertices = parasitic_vector<vec3f>(p, mh->num_vertices);
    texcoords = parasitic_vector<vec2f>(p, mh->num_texcoords);
    normals = parasitic_vector<vec3f>(p, mh->num_normals);
    indices = parasitic_vector<vec3i>(p, mh->num_indices);
}


/**
 * Load an obj file and convert it to a blockloader file.
 */
void mesh::load_obj(const char filename[], int flags) {
    auto t0 = chrono::high_resolution_clock::now();
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, 
        aiProcess_Triangulate |           // Ensure all faces are triangles
        aiProcess_JoinIdenticalVertices | // Merge identical vertices
        aiProcess_PreTransformVertices |  // Apply node transformations
        aiProcess_OptimizeMeshes);        // Reduce number of meshes/draw calls
    
    if (scene == nullptr)
        throw "parse exception";

    vector<mesh_entry> meshes;
    meshes.reserve(scene->mNumMeshes);
    vector<material_entry> materials;
    materials.reserve(scene->mNumMeshes);
    uint32_t total_vertices = 0;
    uint32_t total_indices = 0;
    uint32_t total_texcoords = 0;
    uint32_t total_normals = 0;
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* m = scene->mMeshes[i];
        total_vertices += m->mNumVertices;
        total_indices += m->mNumFaces * 3;
        if (m->HasTextureCoords(0))
            total_texcoords += m->mNumVertices;
        if (m->HasNormals())
            total_normals += m->mNumVertices;
    }

    vector<vec3f> vertices;
    vertices.reserve(total_vertices);
    vector<vec2f> texcoords;
    texcoords.reserve(total_texcoords);
    vector<vec3f> normals;
    normals.reserve(total_normals);
    vector<vec3i> indices;
    indices.reserve(total_indices);
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* m = scene->mMeshes[i];
        //cout << "Mesh " << i << " has vert=" 
        //     << m->mNumVertices << " faces=" << m->mNumFaces << endl;
        uint32_t num_vertices = m->mNumVertices;
        uint32_t num_indices = m->mNumFaces; // indices stores 3 per entry
        materials.emplace_back(m->mMaterialIndex);
        meshes.emplace_back(m->mMaterialIndex,
                            num_indices,
                            num_vertices,
                            num_indices);
        for (unsigned int j = 0; j < m->mNumVertices; j++) {
            vertices.emplace_back(m->mVertices[j].x, m->mVertices[j].y, m->mVertices[j].z);
        }
        if (m->HasTextureCoords(0)) {
            for (unsigned int j = 0; j < m->mNumVertices; j++) {
                texcoords.emplace_back(m->mTextureCoords[0][j].x, m->mTextureCoords[0][j].y);
            }
        }
        if (m->HasNormals()) {
            for (unsigned int j = 0; j < m->mNumVertices; j++) {
                normals.emplace_back(m->mNormals[j].x, m->mNormals[j].y, m->mNormals[j].z);
            }
        }
        for (unsigned int j = 0; j < m->mNumFaces; j++) {
            aiFace& f = m->mFaces[j];
            indices.emplace_back(f.mIndices[0], f.mIndices[1], f.mIndices[2]);
        }
    }
    auto t1 = chrono::high_resolution_clock::now();
    cout << fixed << setprecision(3) 
         << duration_cast<microseconds>(t1 - t0).count() / 1000.0 
         << "ms" << endl;

    size_t total_size = sizeof(mesh_header) +
                       meshes.size() * sizeof(mesh_entry) +
                       materials.size() * sizeof(material_entry) +
                       vertices.size() * sizeof(vec3f) +
                       texcoords.size() * sizeof(vec2f) +
                       normals.size() * sizeof(vec3f) +
                       indices.size() * sizeof(vec3i);
    bl.grow(total_size);
    mesh_header* mh = (mesh_header*)bl.start_data();
    mh->num_meshes = meshes.size();
    mh->num_materials = materials.size();
    mh->num_vertices = vertices.size();
    mh->num_texcoords = texcoords.size();
    mh->num_normals = normals.size();
    mh->num_indices = indices.size();
    bl.append(meshes.data(), meshes.size() * sizeof(mesh_entry));
    bl.append(materials.data(), materials.size() * sizeof(material_entry));
    bl.append(vertices.data(), vertices.size() * sizeof(vec3f));
    bl.append(texcoords.data(), texcoords.size() * sizeof(vec2f));
    bl.append(normals.data(), normals.size() * sizeof(vec3f));
    bl.append(indices.data(), indices.size() * sizeof(vec3i));
}

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

void mesh::save(const char filename[]) {
    bl.save(filename);
}

