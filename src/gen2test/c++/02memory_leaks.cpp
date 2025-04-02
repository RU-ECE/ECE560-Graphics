#include <iostream>
//RAII: Resource Acquisition Is Initialization

class Resource {
    int* x;
public:
    Resource() {
        x = new int[32000];
        if (x == nullptr) {
            throw std::bad_alloc();
        }
    }
    Resource(const Resource& other) = delete;
    Resource& operator=(const Resource& other) = delete;
    ~Resource() {
        delete[] x;
    }
};

void f() {
    int* x = new int[32000];
    // forget to delete... memory leak!
}
int main() {
    for (int i = 0; i < 1'000'000; i++) {
        f();
    }
    Resource r;
 //   Resource r2 = r;
}