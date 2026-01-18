#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

struct bound_rect {
    float x, y, width, height;
};

class shape {
private:
    float x, y;
public:
    shape(float x, float y) : x(x), y(y) {}
    virtual void draw() = 0;
    virtual bound_rect get_bounds() = 0; // fast test to rule out intersection
    virtual bool is_point_inside(float px, float py) = 0; // slow test to check if point is inside
    virtual ~shape() = 0; // virtual destructor
};

class polygon : public shape {
public:
    polygon(float x, float y, std::vector<float> points) : shape(x, y), points(points) {}
    void draw() override; // if you don't override, you cannot instantiate a polygon
    bound_rect get_bounds() override;
    bool is_point_inside(float px, float py) override;
    ~polygon() override;
private:
    std::vector<float> points;
};
class rect : public shape {};
class squiggle : public shape {};

class quadtree {
private:
    static const int toobig = 10;
    struct node {
        node* children[4];
        std::vector<shape*> shapes;
        bound_rect bounds;
        node() {
            for (int i = 0; i < 4; i++) {
                children[i] = nullptr;
            }
        }
        ~node() {
            for (auto s : shapes) {
                delete s;
            }
            for (int i = 0; i < 4; i++) {
                delete children[i];
            }
        }
    };

    node* root;

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
    quadtree(bound_rect bounds) {
        root = new node();
        root->bounds = bounds;
    }

    ~quadtree() {
        delete root;
    }
/*
    0 1
    2 3
*/

    void insert(shape* s) {
        insert(root, s);
    }

};