#include "mesh.hh"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
using namespace std;

/**
 * Load an obj file and convert it to a blockloader file.
 */
void convert_obj_blobj(blockloader& bl, const char filename[], int flags) {
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
    vector<vec2i> index_ranges; // the start and end index of each sequential range of indices
    index_ranges.reserve(total_indices/30);
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
        if (m->mNumFaces > 0) {
            uint32_t check_sequential = m->mFaces[0].mIndices[0];
            uint32_t start_sequential = check_sequential;
            uint32_t end_sequential = check_sequential;
            for (unsigned int j = 0; j < m->mNumFaces; j++) {
                aiFace& f = m->mFaces[j];
                if (f.mIndices[0] != check_sequential || f.mIndices[1] != check_sequential+1 || f.mIndices[2] != check_sequential+2) {
                    end_sequential = check_sequential;
                    indices.emplace_back(f.mIndices[0], f.mIndices[1], f.mIndices[2]);
                    if (end_sequential- start_sequential > 5) {
                        index_ranges.emplace_back(start_sequential, end_sequential);
//                        cerr << "mesh indices sequential from " << start_sequential << " to " << end_sequential << endl;    
                        start_sequential = end_sequential;
                    } else {
                        uint32_t biggest = max(f.mIndices[0], max(f.mIndices[1], f.mIndices[2]));
                        for (unsigned int k = start_sequential; k < check_sequential+3; k++) {
                            indices.emplace_back(f.mIndices[0], f.mIndices[1], f.mIndices[2]);
                            uint32_t biggest = max(f.mIndices[0], max(f.mIndices[1], f.mIndices[2]));
                        }
                        start_sequential = check_sequential+4;
                    }
                }

                check_sequential += 3;
            }
        }
    }
    auto t1 = chrono::high_resolution_clock::now();
    cout << fixed << setprecision(3) 
         << duration_cast<microseconds>(t1 - t0).count() / 1000.0 
         << "ms" << endl;

    size_t total_size = sizeof(mesh::mesh_header) +
                       meshes.size() * sizeof(mesh_entry) +
                       materials.size() * sizeof(material_entry) +
                       vertices.size() * sizeof(vec3f) +
                       texcoords.size() * sizeof(vec2f) +
                       normals.size() * sizeof(vec3f) +
                       indices.size() * sizeof(vec3i);
    bl.grow(total_size);
    mesh::mesh_header* mh = (mesh::mesh_header*)bl.start_data();
    mh->num_meshes = meshes.size();
    mh->num_materials = materials.size();
    mh->num_vertices = vertices.size();
    mh->num_texcoords = texcoords.size();
    mh->num_normals = normals.size();
    mh->num_indices = indices.size();
    mh->num_index_ranges = index_ranges.size();
    bl.append(meshes.data(), meshes.size() * sizeof(mesh_entry));
    bl.append(materials.data(), materials.size() * sizeof(material_entry));
    bl.append(vertices.data(), vertices.size() * sizeof(vec3f));
    bl.append(texcoords.data(), texcoords.size() * sizeof(vec2f));
    bl.append(normals.data(), normals.size() * sizeof(vec3f));
    bl.append(indices.data(), indices.size() * sizeof(vec3i));
    bl.append(index_ranges.data(), index_ranges.size() * sizeof(uint32_t));
}



int main(int argc, char* argv[]) {
    if (argc < 3) {
//        cerr << "Usage: " << argv[0] << " <objfile> <blfile>" << endl;
//        return 1;
    }
    const char* objfile = argc >= 2 ? argv[1] : "models/antique_ceramic_vase_01_4k.obj";
    const char* blfile = argc >= 3 ? argv[2] : "models/antique_vase.blobj";
    blockloader bl(1024, 0, 0, 0, 0, 0);
    convert_obj_blobj(bl, objfile, 0);

    
//    m.save(blfile); // save to blockloader binary object model 
    return 0;
}
