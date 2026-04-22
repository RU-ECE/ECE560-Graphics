// sphere_no_index.hpp
#pragma once
/*

   This is not a good idea. 6 vertices per quad is just stupid. 

*/
#include <vector>
#include <cmath>
#include <cstdint>

using namespace std;

// --- Vertex formats ---
struct VertexXYZUV {
    float x, y, z;
    float u, v;
};

struct VertexXYZRGB {
    float x, y, z;
    float r, g, b;
};

struct VertexXYZUVN {
    float x, y, z;
    float u, v;
    float nx, ny, nz;
};

// --- Sphere Generator (NO INDICES: fully expanded triangles) ---
class SphereNoIndex {
public:
    SphereNoIndex(uint32_t slices, uint32_t stacks, float radius = 1.0f)
        : slices(slices), stacks(stacks), radius(radius) {}

    // --- XYZUV ---
    void generateXYZUV(vector<VertexXYZUV>& vertices) {
        vertices.clear();
        vertices.reserve(slices * stacks * 6); // 2 triangles per quad

        for (uint32_t i = 0; i < stacks; i++) {
            float v0 = float(i) / stacks;
            float v1 = float(i + 1) / stacks;

            float phi0 = v0 * M_PI;
            float phi1 = v1 * M_PI;

            for (uint32_t j = 0; j < slices; j++) {
                float u0 = float(j) / slices;
                float u1 = float(j + 1) / slices;

                float theta0 = u0 * 2.0f * M_PI;
                float theta1 = u1 * 2.0f * M_PI;

                // --- compute 4 corners of quad ---
                auto p = [&](float phi, float theta) {
                    float x = radius * sin(phi) * cos(theta);
                    float y = radius * cos(phi);
                    float z = radius * sin(phi) * sin(theta);
                    return VertexXYZUV{x, y, z, theta / (2.0f * M_PI), phi / M_PI};
                };

                VertexXYZUV a = p(phi0, theta0);
                VertexXYZUV b = p(phi0, theta1);
                VertexXYZUV c = p(phi1, theta0);
                VertexXYZUV d = p(phi1, theta1);

                // --- emit triangles (handle poles implicitly) ---

                // top cap: skip upper triangle when phi0 == 0
                if (i != 0) {
                    vertices.push_back(a);
                    vertices.push_back(c);
                    vertices.push_back(b);
                }

                // bottom cap: skip lower triangle when phi1 == PI
                if (i != stacks - 1) {
                    vertices.push_back(b);
                    vertices.push_back(c);
                    vertices.push_back(d);
                }
            }
        }
    }

    // --- XYZRGB (Gouraud) ---
    void generateXYZRGB(vector<VertexXYZRGB>& vertices) {
        vertices.clear();
        vertices.reserve(slices * stacks * 6);

        for (uint32_t i = 0; i < stacks; i++) {
            float v0 = float(i) / stacks;
            float v1 = float(i + 1) / stacks;

            float phi0 = v0 * M_PI;
            float phi1 = v1 * M_PI;

            for (uint32_t j = 0; j < slices; j++) {
                float u0 = float(j) / slices;
                float u1 = float(j + 1) / slices;

                float theta0 = u0 * 2.0f * M_PI;
                float theta1 = u1 * 2.0f * M_PI;

                auto p = [&](float phi, float theta) {
                    float x = radius * sin(phi) * cos(theta);
                    float y = radius * cos(phi);
                    float z = radius * sin(phi) * sin(theta);

                    // simple color from normalized position
                    float r = (x / radius + 1.0f) * 0.5f;
                    float g = (y / radius + 1.0f) * 0.5f;
                    float b = (z / radius + 1.0f) * 0.5f;

                    return VertexXYZRGB{x, y, z, r, g, b};
                };

                VertexXYZRGB a = p(phi0, theta0);
                VertexXYZRGB b = p(phi0, theta1);
                VertexXYZRGB c = p(phi1, theta0);
                VertexXYZRGB d = p(phi1, theta1);

                if (i != 0) {
                    vertices.push_back(a);
                    vertices.push_back(c);
                    vertices.push_back(b);
                }

                if (i != stacks - 1) {
                    vertices.push_back(b);
                    vertices.push_back(c);
                    vertices.push_back(d);
                }
            }
        }
    }

    // --- XYZUV + normals ---
    void generateXYZUVN(vector<VertexXYZUVN>& vertices) {
        vertices.clear();
        vertices.reserve(slices * stacks * 6);

        for (uint32_t i = 0; i < stacks; i++) {
            float v0 = float(i) / stacks;
            float v1 = float(i + 1) / stacks;

            float phi0 = v0 * M_PI;
            float phi1 = v1 * M_PI;

            for (uint32_t j = 0; j < slices; j++) {
                float u0 = float(j) / slices;
                float u1 = float(j + 1) / slices;

                float theta0 = u0 * 2.0f * M_PI;
                float theta1 = u1 * 2.0f * M_PI;

                auto p = [&](float phi, float theta) {
                    float x = radius * sin(phi) * cos(theta);
                    float y = radius * cos(phi);
                    float z = radius * sin(phi) * sin(theta);

                    float nx = x / radius;
                    float ny = y / radius;
                    float nz = z / radius;

                    return VertexXYZUVN{
                        x, y, z,
                        theta / (2.0f * M_PI),
                        phi / M_PI,
                        nx, ny, nz
                    };
                };

                VertexXYZUVN a = p(phi0, theta0);
                VertexXYZUVN b = p(phi0, theta1);
                VertexXYZUVN c = p(phi1, theta0);
                VertexXYZUVN d = p(phi1, theta1);

                if (i != 0) {
                    vertices.push_back(a);
                    vertices.push_back(c);
                    vertices.push_back(b);
                }

                if (i != stacks - 1) {
                    vertices.push_back(b);
                    vertices.push_back(c);
                    vertices.push_back(d);
                }
            }
        }
    }

private:
    uint32_t slices;
    uint32_t stacks;
    float radius;
};