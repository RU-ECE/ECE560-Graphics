#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

/*
    Create a simple GLUT window. It doesn't draw anything, not even blank.
    Resizing is handled by the operating system. We have zero control
    over the look.
*/
int main(int argc, char** argv) {
    glutInit(&argc, argv); // initialize the GLUT library
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Empty window with no drawing");

    glutMainLoop(); // waits and draws repeatedly
    return 0;
}