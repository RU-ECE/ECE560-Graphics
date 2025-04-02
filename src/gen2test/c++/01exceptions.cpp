#include <iostream>

int g(int x) {
    if (x < 0) {
        throw "tantrum";
    }
    return 3*x + 4;
}

int f(int x) {
    return g(x+1);
}

int main() {
    try {
        f(1);
    } catch (const char* msg) {
        std::cout << msg << std::endl;
    }
}