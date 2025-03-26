#include <iostream>
#include "mesh.hh"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
//        cerr << "Usage: " << argv[0] << " <objfile> <blfile>" << endl;
//        return 1;
    }
    const char* objfile = argc >= 2 ? argv[1] : "models/antique_ceramic_vase_01_4k.obj";
    const char* blfile = argc >= 3 ? argv[2] : "models/antique_vase.blobj";
    blockloader bl(1024, 0, 0, 0, 0, 0);
    mesh m(bl, objfile);
    m.save(blfile); // save to blockloader binary object model 
    return 0;
}
