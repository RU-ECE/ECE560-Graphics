#pragma once
#include <vector>

class bezier2d {
private:
    float ax, bx, cx, dx;
    float ay, by, cy, dy;
public:
    bezier2d(const float p[], uint32_t n) {
        // x(t) = ((ax * t + bx) * t + cx) * t + dx;
        // y(t) = ((ay * t + by) * t + cy) * t + dy; 
        // x'(t) = 3 * (ax * t + bx) * t + cx;
        // y'(t) = 3 * (ay * t + by) * t + cy;

        // @t=0 ax*0 + bx*0 + cx*0 + dx = dx =p[3]
        // @t=0 ay*0 + by*0 + cy*0 + dy = dy = p[7]
        // @t=1 ax + bx + cx + dx = p[2] ==> ax + bx + cx = p[2] - dx
        // @t=1 ay + by + cy + dy = p[3] ==> ay + by + cy = p[3] - dy
        // 
        dx = p[6], dy = p[7]; // @t=0
        ax = p[0], ay = p[1];
        bx = p[2] - dx, by = p[3] - dy;
        cx = p[4] - dx, cy = p[5] - dy;
    }

    double x() const {
        return 
    }
}