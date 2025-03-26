#include "bezier.hh"
using namespace std;

int main() {
    bezier2d b1(0,0, 0.25,1.0, 0.75,1.0, 1,0);
    cout << "b1=" << b1 << '\n';
    b1.print(); // print all values of the bezier from t=0.0 to t=1.0

    
}