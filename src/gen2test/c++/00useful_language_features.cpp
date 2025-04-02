#include <iostream>

bool g() {
    if (rand() % 2 == 0) {
        return false;
    }
    std::cout << "g" << std::endl;
    return true;
}

int f() {
    if (!g()) {
        return 0;
    }
    return 1;
}

/*
 old error handling: using return values
 problem 1: huge amount of code
     We want an automated way to handle errors
 problem 2: sometimes an error can be misinterpreted as a normal return value


*/
int myfunc(int x) {
    if (x < 0) {
        return -1;
    }
    return sqrt(x);
}
int main() {
    f();
    int z = myfunc(-3); // if we forget to check the return value, then the answer is wrong
}