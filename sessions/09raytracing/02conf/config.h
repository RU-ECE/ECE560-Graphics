#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
using namespace std;

class config {
public:
    config(const string& filename);

    template<typename T>
    T expect(ifstream& f, const string& expected);

private:
    double defocus_angle;
    uint32_t img_width;
    uint32_t img_height;
    uint32_t samples_per_pixel;
    uint32_t max_depth;
};

template<>
inline uint32_t config::expect<uint32_t>(ifstream& f, const string& expected) {
    string name, s;
    f >> name >> s;
    if (name != expected)
        cerr << "Expected " << expected << " but got " << name << '\n';
    return atoi(s.c_str());
}

template<>
inline double config::expect<double>(ifstream& f, const string& expected) {
    string name, s;
    f >> name >> s;
    if (name != expected)
        cerr << "Expected " << expected << " but got " << name << '\n';
    return atof(s.c_str());
}

inline config::config(const string& filename) {
    ifstream f(filename);
    defocus_angle = expect<double>(f, "defocus_angle");
    img_width = expect<uint32_t>(f, "img_width");
    img_height = expect<uint32_t>(f, "img_height");
    samples_per_pixel = expect<uint32_t>(f, "samples_per_pixel");
    max_depth = expect<uint32_t>(f, "max_depth");
}
