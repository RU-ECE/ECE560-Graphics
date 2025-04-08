#pragma once
#include "graphics_core.hh"

/**
    Represent a maze in a grid similar to old castle wolfenstein
    a 2D grid of cells
    each cell has 4 walls
    the player can move around the maze
    the player can turn left, right go forward and backward
*/
class maze {
private:
    uint32_t  program_id;
    uint32_t  vao;
    uint32_t  vbo;
    struct wall {
        uint8_t material;    // type of material
        uint8_t texture_id;  // which texture to use
        uint8_t strength;    // damage capacity
        uint8_t flags;       // properties
    };

    uint8_t* path;          // 0 = solid rock, 1 = open space
    wall* vertical_walls;    // (width+1) * height walls
    wall* horizontal_walls;  // width * (height+1) walls
    
    // Helper to access walls
    wall& get_vertical_wall(uint32_t x, uint32_t y) {
        return vertical_walls[x + y * (width + 1)];
    }
    
    wall& get_horizontal_wall(uint32_t x, uint32_t y) {
        return horizontal_walls[x + y * width];
    }

    void set_path(loc L, uint8_t value) {
        path[L.x + L.y * width] = value;
    }

    struct cell {
        wall walls[4];      // north, east, south, west walls
        uint8_t floor_tex;  // floor texture
        uint8_t ceil_tex;   // ceiling texture
    };

    cell* map;             // replace uint32_t* map
    uint32_t  width;
    uint32_t  height;
    struct loc {
        uint32_t x;
        uint32_t y;
    };
    loc start;
    loc end;
    loc pick_random_edge() {
        uint32_t xy = dist(rng);
        uint32_t x, y;
        if (xy % 2 == 0) {
            uniform_int_distribution<uint32_t> yedge(0, height);
            y = yedge(rng);
            x = (xy == 0) ? 0 : width-1;
            return loc{x, y};
        } else {
            uniform_int_distribution<uint32_t> xedge(0, width);
            if (xy == 1)
                x = xedge(rng);
            uint32_t y = dist(rng);
            return loc{x, y};
        }
    }
    void set(loc L, const Cell& value) {
        map[L.x + L.y * width] = value;
    }

    static constexpr uint8_t material_stone = 0;
    static constexpr uint8_t material_wood = 1;
    static constexpr uint8_t material_metal = 2;
    
    static constexpr uint8_t flag_flammable = 1;
    static constexpr uint8_t flag_transparent = 2;
    static constexpr uint8_t flag_breakable = 4;
public:
    maze(uint32_t width, uint32_t height) : width(width), height(height) {
        program_id = build_prog(vs_maze, fs_maze);
        make_vao_vbo(vao, vbo, xyuv.data(), xyuv.size());
        path = new uint8_t[width * height]();  // zero-initialized
        vertical_walls = new wall[(width + 1) * height]();
        horizontal_walls = new wall[width * (height + 1)]();
        map = new Cell[width * height];
        for (uint32_t i = 0; i < width * height; i++) {
            map[i] = Cell{};
        }
        // pick a random starting point on the edge of the maze
        start = pick_random_edge();
        end = pick_random_edge();
        set(start, Cell{});
        set(end, Cell{});
        // drill out random paths to construct the maze
        while (true) {
            
        }
    }
    ~maze() {
        delete[] path;
        delete[] vertical_walls;
        delete[] horizontal_walls;
        glDeleteProgram(program_id);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
    void render();
};

maze::~maze() {
    glDeleteProgram(program_id);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}