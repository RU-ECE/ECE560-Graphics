#include <iostream>
#include "geometry.hpp"

using namespace std;
/*
           p1             p2


     p0                         p3

*/

float f(float x) {
    return x*x*x;
}

float g(float x) {
    return sqrt(x); // int of sqrt(x) from 1 to 2 is 2/3 * (2^(3/2) - 1)
}

// f(1) = 1
// f(0) = 0
// integral(f, 0, 1) = (1^3 - 0^3) / 4 = 1/4 = .25
int main() {
    cout << gauss_legendre5(f, 0, 1) << '\n';;
    cout << gauss_legendre5(g, 1, 2) << '\n';;

    Bezier b1({{0, 0, 0}, {2, 2, 0}, {4, 2, 0}, {6, 0, 0}});
    for (float t = 0; t <= 1; t += 0.1) {
        auto p = b1.evaluate(t);
        cout << "t: " << t << " p: " << p << endl;
    }
}