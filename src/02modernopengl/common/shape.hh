#pragma once

/**
* Shape class
* static methods to create shapes and return the integer handle to the 
* vertexes and indices
*/

class shape {
    public:
        uint32_t vao;
        uint32_t vbo;
        uint32_t ibo;
        uint32_t indexSize;
        static shape gen_sphere(int lat_res, int lon_res);
        static shape gen_cube();
        static shape gen_cylinder(uint32_t ring_res);
        static shape gen_cone(uint32_t ring_res);
        static shape gen_torus(float tube_radius, int lat_res, int lon_res);
        static shape gen_grid(int gridX, int gridY);
        static shape gen_circle(int circle_res);

        shape(const float vert[], const uint32_t vert_size,
            const uint32_t indices[], const uint32_t index_size);
// each render method should have an associated precomputed shader program
// should not be needed by the caller
        void render_textured(uint32_t texture_id);
        void render_colored();
}
