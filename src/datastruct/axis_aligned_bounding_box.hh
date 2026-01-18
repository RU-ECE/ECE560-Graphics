#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

class shape {
private:
    float x, y, z;
public:
    shape(float x, float y, float z) : x(x), y(y), z(z) {}
    float get_x() const { return x; }
    float get_y() const { return y; }
    float get_z() const { return z; }
    virtual void draw() = 0;
    virtual ~shape() = 0;
};

class axis_aligned_bounding_box {
private:
    float x, y, z, width, height, depth;
    float dx, dy, dz;
    int n;
    // first find the right box, inside it is a list of shapes
    std::vector< std::vector< shape*>>> shapes;
public:
    axis_aligned_bounding_box(float x, float y, float z, float width, float height, float depth, int n) 
    : x(x), y(y), z(z), width(width), height(height), depth(depth), n(n) {
        dx = width / n;
        dy = height / n;
        dz = depth / n;
    }
    ~axis_aligned_bounding_box() {}
    float get_x() const { return x; }
    float get_y() const { return y; }
    float get_z() const { return z; }
    float get_width() const { return width; }
    float get_height() const { return height; }
    float get_depth() const { return depth; }
    void add(shape* s) {
        int i = (s->get_x() - x) / dx;
        int j = (s->get_y() - y) / dy;
        int k = (s->get_z() - z) / dz;
        if (i < 0 || i >= n || j < 0 || j >= n || k < 0 || k >= n) {
            std::cerr << "Shape is out of bounds" << std::endl;
            return;
        }
        //int boxid = i * n * n + j * n + k;
        int boxid = (i * n + j) * n + k; // Horner's form
        boxes[boxid].push_back(s);
    }
};