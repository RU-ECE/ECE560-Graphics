#pragma once
#include <vector>
#include <cstdint>

struct vec3d {
    float x, y, z;
};

struct point3d { // 3d point with color
    float x, y, z;
    float r, g, b;
};

class Shape {
public:
    static std::vector<point3d> sphere(uint32_t latRes, uint32_t longRes, float radius);
    static std::vector<point3d> cube(float size);
    static std::vector<point3d> cylinder(uint32_t res, float radius, float height);
    static std::vector<point3d> cone(uint32_t res, float radius, float height);
    static std::vector<point3d> torus(uint32_t latRes, uint32_t longRes, float innerRadius, float outerRadius);
    static std::vector<point3d> disk(uint32_t res, float radius);

    static void heatmap(const std::vector<point3d>& vertices, std::vector<uint32_t>& indices , float minVal, float maxVal);
};