#pragma once

#include <iostream>
#include <vector>
#include <cmath>

class vec3d {
public:
    float x, y, z;
    vec3d(float x, float y, float z) : x(x), y(y), z(z) {}
    friend vec3d operator+(const vec3d& a, const vec3d& b) {
        return vec3d(a.x + b.x, a.y + b.y, a.z + b.z);
    }
    friend vec3d operator-(const vec3d& a, const vec3d& b) {
        return vec3d(a.x - b.x, a.y - b.y, a.z - b.z);
    }
    vec3d operator*(float scalar) const {
        return vec3d(x * scalar, y * scalar, z * scalar);
    }
    friend std::ostream& operator<<(std::ostream& os, const vec3d& v) {
        return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    }
};


/*
    3D Bezier curve implementation
    You must be able to move the object at a constant speed along the curve.
    This means that you need to calculate the length of the curve
*/
class Bezier {
private:
    std::vector<vec3d> c;
public:
    Bezier(const std::vector<vec3d>& control_points) : c(control_points) {
        if (c.size() != 4) {
            throw std::invalid_argument("Bezier curve must have 4 control points");
        }
    }
    vec3d evaluate(float t) const {
        float u = 1 - t;
        return c[0] * (u * u * u) + c[1] * (3 * u * u * t) + c[2] * (3 * u * t * t) + c[3] * (t * t * t);
        // f(t) = ((d*t + c) * t + b) * t + a;   
        // f(0) = 0           0        0 + a = p0.x
        // f(1) =  d + c + b = p3.x - p0.x
        // Horner's form    (c[3] * t + c[2]) * t + c[1]) * t + c[0]);
    }
    float length() const { return length(0, 1); }

    // compute the derivative dx(t) of the curve, where x is the x component 
    float dx(float t) const {
        float u = 1 - t;
        return 3 * (c[1].x - c[0].x) * (u * u) + 6 * (c[2].x - c[1].x) * (u * t) + 3 * (c[3].x - c[2].x) * (t * t);
    }

        // compute the derivative dy(t) of the curve, where x is the x component 
    float dy(float t) const {
        float u = 1 - t;
        return 3 * (c[1].y - c[0].y) * (u * u) + 6 * (c[2].y - c[1].y) * (u * t) + 3 * (c[3].y - c[2].y) * (t * t);
    }

    // compute the derivative dz(t) of the curve, where x is the x component 
    float dz(float t) const {
        float u = 1 - t;
        return 3 * (c[1].z - c[0].z) * (u * u) + 6 * (c[2].z - c[1].z) * (u * t) + 3 * (c[3].z - c[2].z) * (t * t);
    }

    float speed(float t) const {
        const float tx = dx(t), ty = dy(t), tz = dz(t); 
        return sqrt(tx*tx + ty*ty + tz*tz);
    }
    /*
        Implement the 5-point Gauss-Legendre quadrature to calculate the length of the curve
    */
    float gauss_legendre5(float t0, float t1) const {
        // x is the position relative to the center of the interval, w is the weight
        const static float x[] = {0, 0.5384693101056831, 0.9061798459386640};
        const static float w[] = {0.5688888888888889, 0.4786286704993665, 0.2369268850561891};

        float h = (t1 - t0)/2;
        float tmid = 0.5 * (t0 + t1); // halfway
        float ta = tmid - x[1]*h, tb = tmid + x[1]*h;
        float tc = tmid - x[2]*h, td = tmid + x[2]*h;
        float len = (w[0] * speed(tmid) + w[1] * (speed(ta) + speed(tb)) + w[2] * (speed(tc) + speed(td)))*h;
        return len;
    }

    float length(float t0, float t1) const {
        return gauss_legendre5(t0, t1);
    }
};


template<typename Func>
float gauss_legendre5(Func f, float a, float b) {
    // x is the position relative to the center of the interval, w is the weight
    const static float x[] = {0, 0.5384693101056831, 0.9061798459386640};
    const static float w[] = {0.5688888888888889, 0.4786286704993665, 0.2369268850561891};

    float h = (b - a)/2; // this is the width of the slice you are consider /2
    float tmid = 0.5 * (a + b); // halfway
    float ta = tmid - x[1]*h, tb = tmid + x[1]*h;
    float tc = tmid - x[2]*h, td = tmid + x[2]*h;
    return (w[0] * f(tmid) + w[1] * (f(ta) + f(tb)) + w[2] * (f(tc) + f(td)))*h;
}



/*
    TODO: A path is a sequence of bezier, arcs, splines, that define a continuous path in 3D space.
*/
class Path {

};

/*
    Quaternion implementation
*/
class Quaternion {

};
