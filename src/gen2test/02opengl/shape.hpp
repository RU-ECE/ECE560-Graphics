#pragma once
#include <vector>
#include <cstdint>

struct pt_tex2d { // 3d point with 2D texture coordinates
    float x, y, z, u, v;
};

struct pt_tex2d_norm { // 3d point + uv + normal (for lighting)
    float x, y, z, u, v, nx, ny, nz;
};

struct pt_val { // 3d point with value to look up color in 1D texture
    float x, y, z, val;
};

class shape {
public:
/*
    HW: complete the implementation of the sphere.
    you will have to verify that the indices are correct and create
    the indices to correctly connect all the dots. Assume we are using
    GL_TRIANGLE_STRIP to draw the sphere.
    Use degenerate triangles to connect the strips.
    Build the index vector i
*/
    static void gen_sphere(int lat_res, int lon_res,
        std::vector<pt_tex2d>& v, std::vector<uint32_t>& i) {
        v.resize(2 * lat_res * lon_res); // preallocate the right number ofvertices
        i.resize(2 * lat_res * lon_res); // preallocate the right number of indices
        double a = 0, da = 2 * M_PI / lon_res;
        for (int i = 0, c = 0; i < lat_res; i++) {
            double lat = -M_PI/2 + M_PI * i / lat_res;
            double z = sin(lat);
            double r = cos(lat);
            for (int j = 0; j < lon_res; j++, c++) {
                double t = a + da * j;
                v[c] = {(float)(r * cos(t)), (float)(r * sin(t)), (float)z, (float)(t / (2 * M_PI)), (float)(lat / M_PI)};
            }
        }
        // HW: add all the indices
    }

/**
    Example: build a cylinder with ring_res segments around the circle.
    This means we need ring_res + 1 edges to connect back to start.
    2 points per iteration (one at y=0, the other at y=1)
    This is scaled to a unit radius and unit height, use transforms

*/
    static std::vector<pt_tex2d> gen_cylinder_textured(uint32_t ring_res) {
        std::vector<pt_tex2d> v(2 * (ring_res + 1));
        double a = 0, da = 2 * M_PI / ring_res;
        for (int i = 0, c = 0; i <= ring_res; i++, c++, a += da) {
            double x = cos(a), z = sin(a);
            v[c++] = {(float)x, 0.f, (float)z, (float)a, 0.f};
            v[c++] = {(float)x, 1.f, (float)z, (float)a, 1.f};
        }
        return v;
    }

    static std::vector<pt_tex2d_norm> gen_cylinder_textured_with_normals(uint32_t ring_res) {
        std::vector<pt_tex2d_norm> v(2 * (ring_res + 1));
        double a = 0, da = 2 * M_PI / ring_res;
        for (uint32_t i = 0, c = 0; i <= ring_res; i++, a += da) {
            float x = (float)cos(a), z = (float)sin(a);
            v[c++] = {x, 0.f, z, (float)a, 0.f, x, 0.f, z};
            v[c++] = {x, 1.f, z, (float)a, 1.f, x, 0.f, z};
        }
        return v;
    }
};