#include <iostream>
#include <vector>

int main() {
    std::vector<int> v;
    try {
        for (int i = 0; i < 1000000; i++) {
            if (i == 23099) {
                throw "ugh!";
            }
            v.push_back(i);
        }
    } catch (const char* msg) {
        std::cout << msg << std::endl;
    }
    std::cout << v.size() << '\n';
}