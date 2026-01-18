#pragma once

/*
 bring in standard utilities including
    iostream, cmath, random 
    vector
    array2d
    vec3d
    vec2d
    etc.
*/
#include "graphics_core.hh"

template<typename T>
class array2d {
private:
    vector<T> data;
    uint32_t w, h;
public:
    array2d(uint32_t width, uint32_t height) : data(width*height), w(width), h(height) {}
    uint32_t width() const { return w; }
    uint32_t height() const { return h; }

    // index the array with x,y coordinates
    T operator()(uint32_t x, uint32_t y) const {
        return data[y*w + x];
    }
    T& operator()(uint32_t x, uint32_t y) {
        return data[y*w + x];
    }
    // provide direct access to the data array linearly
    T& operator[](uint32_t i) {
        return data[i];
    }
    // this will only work if T supports the << operator
    friend std::ostream& operator<<(std::ostream& os, const array2d<T>& arr) {
        for (uint32_t y = 0; y < arr.h; y++) {
            for (uint32_t x = 0; x < arr.w; x++) {
                os << arr(x, y) << " ";
            }
            os << std::endl;
        }
        return os;
    }
};

class terrain {
private:
    array2d<float> height_map;
    array2d<vec2d> surface;
    uint32_t w, h; // width and height of the terrain
    uint32_t vao; // container for the data
    uint32_t vbo_xy; // the 2d component of the points x,y
    uint32_t vbo_z; // the z component of the points (taken from the height map)
    uint32_t vbo_indices; // the indices of the points used to make the grid out of a triangle strip

    float apply_filter(uint32_t x, uint32_t y, float filter) {
        float prev_val;
        float cur_val = height_map(x, y);
        float new_val = cur_val;

        // check left neighbor
        if (x > 0) {
            prev_val = height_map(x-1, y);
            new_val = filter * prev_val + (1 - filter) * cur_val;
        }
        // check right neighbor
        if (x < w-1) {
            prev_val = height_map(x+1, y);
            new_val = filter * prev_val + (1 - filter) * new_val;
        }
        // check top neighbor
        if (y > 0) {
            prev_val = height_map(x, y-1);
            new_val = filter * prev_val + (1 - filter) * new_val;
        }
        // check bottom neighbor
        if (y < h-1) {
            prev_val = height_map(x, y+1);
            new_val = filter * prev_val + (1 - filter) * new_val;
        }

        return new_val;
    }

    void apply_fir_filter(float filter) {
        // apply the filter to the height map
        for (uint32_t y = 0; y < h; y++) {
            for (uint32_t x = 0; x < w; x++) {
                height_map(x, y) = apply_filter(x, y, filter);
            }
        }
    }

    vec2d get_random_point() {
        return vec2d(gen() % w, gen() % h);
    }

    void gen_random_terrain_points(vec2d& p1, vec2d& p2) {
        p1 = get_random_point();
        p2 = get_random_point();
    }

    void normalize(float new_min, float new_max) {
        float min_val = height_map[0], max_val = min_val;
        const uint32_t size = w*h;
        for (uint32_t i = 0; i < size; i++) {
            if (height_map[i] < min_val) min_val = height_map[i];
            if (height_map[i] > max_val) max_val = height_map[i];
        }
    
        const float m = (new_max - new_min) / (max_val - min_val);
        const float b = new_min - min_val * m;
        for (uint32_t i = 0; i < size; i++) {
            height_map[i] = height_map[i] * m + b; // fast, linear transform
        }
    }

public: 
    terrain(uint32_t w, uint32_t h) : height_map(w, h), w(w), h(h) {
        program = create_program("terrain.vert", "terrain.frag");
    }
    void create_fault_formation(uint32_t iterations,
                                float min_height, float max_height, float filter) {  
        create_fault_formation_internal(iterations, min_height, max_height, filter);
        height_map.normalize(min_height, max_height);
        surface.create_triangle_list();
        apply_fir_filter(filter);
    }

    void create_fault_formation_internal(uint32_t iterations, float min_height, float max_height) {
        float delta_height = max_height - min_height;

        const float inverse_iterations = 1.0f / (float)iterations;
        for (float cur_iter = 0 ; cur_iter < iterations ; cur_iter++) {
            float iteration_ratio = cur_iter * inverse_iterations;
            float height = max_height - iteration_ratio * delta_height;
            vec2d p1, p2;
            gen_random_terrain_points(p1, p2);

            // generate the unit vector from p1 to p2 
            float dir_x = p2.x - p1.x, dir_y = p2.y - p1.y;
            float length = sqrt(dir_x*dir_x + dir_y*dir_y);
            dir_x /= length;
            dir_y /= length;

            for (uint32_t y = 0 ; y < terrain_size ; y++) {
                for (uint32_t x = 0 ; x < terrain_size ; x++) {
                    float dir_x_in = x - p1.x;
                    float dir_y_in = y - p1.y;
                    float cross_product = dir_x_in * dir_y - dir_x * dir_y_in;

                    // create a "fault line" by moving the height up or down
                    if (cross_product > 0) {
                        height_map(x,y) += height;
                    }
                }
            }
        }        
    }

    // create a triangle strip from the height map
    void create_triangle_list() {
        surface.clear();
        for (uint32_t y = 0; y < h-1; y++) {
            for (uint32_t x = 0; x < w-1; x++) {
                // create a the quad using two pairs in a triangle strip
                surface.emplace_back(x, y);
                surface.emplace_back(x, y+1);
                surface.emplace_back(x+1, y);
                surface.emplace_back(x+1, y+1);
            }
            // add degenerate triangles to close the strip
            surface.emplace_back(w-1, y);
            surface.emplace_back(w-1, y+1);
        }
        // the last degenerate triangles are not needed, remove them
        surface.pop_back();
        surface.pop_back();
        vbo_xy.create_vbo(surface);
        vbo_z.create_vbo(height_map);
        vbo_indices.create_vbo(surface);
        vao.create_vao(vbo_xy, vbo_z, vbo_indices);
    }

    void render(const mat4& transform) {
        glUseProgram(program);
        glUniform1f(glGetUniformLocation(program, "min_height"), 0.0f);
        glUniform1f(glGetUniformLocation(program, "max_height"), 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1, GL_FALSE, transform.data());

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_xy);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_z);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
        glDrawElements(GL_TRIANGLE_STRIP, surface.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};
