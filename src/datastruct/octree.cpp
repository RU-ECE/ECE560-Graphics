#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

struct bound_cube {
    float x, y,z, width, height, depth;
};

struct bound_sphere {
    float x, y, z, r;
};

class shape3d {
private:
    float x, y, z;
public:
    shape3d(float x, float y, float z) : x(x), y(y), z(z) {}
    virtual void draw() = 0;
    virtual bound_cube get_bounds() = 0; // fast test to rule out intersection
    virtual bound_sphere get_sphere_bounds() = 0;
    virtual bool is_point_inside(float px, float py, float pz) = 0; // slow test to check if point is inside
    virtual ~shape3d() = 0; // virtual destructor
};

class octtree {
private:
    static const int toobig = 10;
    struct node {
        node* children[8];
        std::vector<shape3d*> shapes;
        bound_rect bounds;
        node() {
            for (int i = 0; i < 8; i++) {
                children[i] = nullptr;
            }
        }
        ~node() {
            for (auto s : shapes) {
                delete s;
            }
            for (int i = 0; i < 8; i++) {
                delete children[i];
            }
        }
    };

    node root;

    void insert(node* p, shape* s) {
        if (p->shapes.size() < toobig) {
            p->shapes.push_back(s);
            return;
        }
        //TODO: create 4 children
        // TODO: move all shapes into children
        // TODO: insert into correct child
        // TODO: don't use list size, we need a boolean to know whether to insert
        // here or in a child
        if (s->get_x() > p->centerx()) {
            // right side
            if (s->get_y() > p->centery()) { // top side
                insert(p->shapes[1], s);
            } else { // bottom side
                insert(p->shapes[3], s);
            }           
        } else {
            if (s->get_y() > p->centery()) { // top side
                insert(p->shapes[0], s);
            } else { // bottom side
                insert(p->shapes[2], s);
            }
        }
    }
public:
    octtree(bound_cube bounds) {
        root.cube = bounds;
    }

    ~octtree() {

        for (auto s : root.shapes) {
            delete s;
        }
        for (int i = 0; i < 8; i++) {
            delete root.children[i];
        }
    }
    // insert a shape into the octtree, note this time root is not a pointer
    // hence the &root
    void insert(shape* s) {
        insert(&root, s);
    }

// test if a ray hits a shape
    bool hits(float x0, float y0, float z0, float dx, float dy, float dz) {
        return root.hits(x0, y0, z0, dx, dy, dz);
    }
};