// sphere_with_indices.hpp
#pragma once

#include <vector>
#include <cmath>
#include <cstdint>
#include <cstddef>  // offsetof

struct VertexXYZUV  { float x, y, z, u, v; };
struct VertexXYZRGB { float x, y, z, r, g, b; };
struct VertexXYZUVN { float x, y, z, u, v, nx, ny, nz; };

class SphereGenerator {
public:
    SphereGenerator(uint32_t slices, uint32_t stacks, float radius = 1.0f)
        : slices(slices), stacks(stacks), radius(radius) {}

    void generateXYZUV(std::vector<VertexXYZUV>& verts, std::vector<uint32_t>& idx) {
        verts.clear();
        for (uint32_t i = 0; i <= stacks; i++) {
            float phi = (float(i) / stacks) * M_PI;
            float sp = sinf(phi), cp = cosf(phi);
            float tv = float(i) / stacks;
            for (uint32_t j = 0; j <= slices; j++) {
                float theta = (float(j) / slices) * 2.0f * M_PI;
                float st = sinf(theta), ct = cosf(theta);
                verts.push_back({radius*sp*ct, radius*cp, radius*sp*st,
                                 float(j)/slices, tv});
            }
        }
        generateIndices(idx);
    }

    void generateXYZRGB(std::vector<VertexXYZRGB>& verts, std::vector<uint32_t>& idx) {
        verts.clear();
        for (uint32_t i = 0; i <= stacks; i++) {
            float phi = (float(i) / stacks) * M_PI;
            float sp = sinf(phi), cp = cosf(phi);
            for (uint32_t j = 0; j <= slices; j++) {
                float theta = (float(j) / slices) * 2.0f * M_PI;
                float x = radius * sp * cosf(theta);
                float y = radius * cp;
                float z = radius * sp * sinf(theta);
                verts.push_back({x, y, z,
                                 (x/radius + 1.0f) * 0.5f,
                                 (y/radius + 1.0f) * 0.5f,
                                 (z/radius + 1.0f) * 0.5f});
            }
        }
        generateIndices(idx);
    }

    void generateXYZUVN(std::vector<VertexXYZUVN>& verts, std::vector<uint32_t>& idx) {
        verts.clear();
        for (uint32_t i = 0; i <= stacks; i++) {
            float phi = (float(i) / stacks) * M_PI;
            float sp = sinf(phi), cp = cosf(phi);
            float tv = float(i) / stacks;
            for (uint32_t j = 0; j <= slices; j++) {
                float theta = (float(j) / slices) * 2.0f * M_PI;
                float st = sinf(theta), ct = cosf(theta);
                float x = radius*sp*ct, y = radius*cp, z = radius*sp*st;
                verts.push_back({x, y, z, float(j)/slices, tv,
                                 x/radius, y/radius, z/radius});
            }
        }
        generateIndices(idx);
    }

private:
    uint32_t slices, stacks;
    float    radius;

    void generateIndices(std::vector<uint32_t>& idx) {
        idx.clear();
        for (uint32_t i = 0; i < stacks; i++) {
            uint32_t row1 = i * (slices + 1);
            uint32_t row2 = row1 + (slices + 1);
            for (uint32_t j = 0; j < slices; j++) {
                uint32_t a = row1+j, b = row1+j+1;
                uint32_t c = row2+j, d = row2+j+1;
                if (i != 0)          { idx.push_back(a); idx.push_back(c); idx.push_back(b); }
                if (i != stacks - 1) { idx.push_back(b); idx.push_back(c); idx.push_back(d); }
            }
        }
    }
};
