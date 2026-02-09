#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

/*
  Draw a solid red background, supports resizing the windows
*/
void display() {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // set the current drawing color to red
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); // initialize the GLUT library
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Empty window");

    glutDisplayFunc(display); // every time the window needs to be redrawn, call display()
    glutMainLoop(); // waits and draws repeatedly
    return 0;
}