#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

void init() {
//    glEnable(GL_DEPTH_TEST);
}

void display() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // set gray background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(4.0, 2.0, 1e-9);

    // Draw a sphere
    // notice: direction of triangle is counterclockwise
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); // red (OPenGL uses 0.0 to 1.0 portable)
    glVertex3f(-1.0f, -1.0f, 0.0f); //bottom left
    glColor3f(0.0f, 1.0f, 0.0f); // green (OPenGL uses 0.0 to 1.0 portable)
    glVertex3f(1.0f, -1.0f, 0.0f); // botton right
    glColor3f(0.0f, 0.0f, 1.0f); // blue (OPenGL uses 0.0 to 1.0 portable)
    glVertex3f(0.0f, 1.0f, 0.0f);  // top center
    glEnd();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / h, 1.0, 100.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Hello Sphere");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}