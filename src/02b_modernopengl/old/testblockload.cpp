#include "mesh.hh"

int main(int argc, char** argv) {
    const char* filename = argc < 2 ? "buddha.blobj" : argv[1];
    auto t0 = chrono::high_resolution_clock::now();
    blockloader bl(filename);
    auto t1 = chrono::high_resolution_clock::now();
    cout << fixed << setprecision(3) 
         << duration_cast<microseconds>(t1 - t0).count() / 1000.0 
         << "ms" << endl;
    return 0;
}