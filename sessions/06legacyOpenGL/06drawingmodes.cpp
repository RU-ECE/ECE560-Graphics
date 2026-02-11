#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

void init() {
    glEnable(GL_DEPTH_TEST);
}

void regular_polygon(int n, float radius) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < n; i++) {
        float angle = 2.0f * M_PI * i / n;
        glVertex3f(radius * cos(angle), radius * sin(angle), 0.0f);
    }
    glEnd();
}

void display() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // set gray background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    glTranslatef(-4, -3, 0);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 0.0f); // red (OPenGL uses 0.0 to 1.0 portable)
    glVertex3f(-1.0f, -1.0f, 0.0f); //bottom left
    glColor3f(0.0f, 1.0f, 1.0f); // green (OPenGL uses 0.0 to 1.0 portable)
    glVertex3f(1.0f, -1.0f, 0.0f); // botton right
    glColor3f(1.0f, 0.0f, 1.0f); // blue (OPenGL uses 0.0 to 1.0 portable)
    glVertex3f(0.0f, 1.0f, 0.0f);  // top center
    glEnd();
    glPopMatrix();

    glColor3f(0.0f, 0.0f, 0.0f); // all the rest are black

    glPushMatrix();
    glTranslatef(-2, -3, 0);
    // notice: direction of triangle is counterclockwise
    glBegin(GL_LINES);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(0.5f, -0.8f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f); 
    glVertex3f(0.2f, 0.0f, 0.0f); 
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0, -3, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(-1.0f, -1.0f, 0.0f); //bottom left
    glVertex3f(1.0f, -1.0f, 0.0f); // botton right
    glVertex3f(0.0f, 1.0f, 0.0f);  // top center
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.5, -3, 0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-1.0f, -1.0f, 0.0f); //bottom left
    glVertex3f(1.0f, -1.0f, 0.0f); // botton right
    glVertex3f(0.0f, 1.0f, 0.0f);  // top center
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-4, 0, 0);
    regular_polygon(5, 1.0f); // Draw a pentagon
    glPopMatrix();

    // non-convex polygon (concave) does not draw right
    glPushMatrix();
    glTranslatef(-2, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(0.5f, -0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(0.5f, -0.5f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(2, 0, 0);
    glBegin(GL_QUAD_STRIP);
    glColor3f(1.0f, 0.0f, 0.0f); // all the rest are black
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(-1.5f, +1.0f);
    glColor3f(0.5f, 0.0f, 0.0f); // all the rest are black
    glVertex2f(0, -1.2f);
    glVertex2f(0.8f, +1.2f);
    glColor3f(0.0f, 0.0f, 1.0f); // all the rest are black
    glVertex2f(+1.1f, -1.0f);
    glVertex2f(+0.9f, +1.1f);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-4, +2, 0);
    glBegin(GL_QUAD_STRIP);
    glColor3f(1.0f, 0.0f, 0.0f); // the first box is red
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(-1.5f, +1.0f);
    glVertex2f(0, -1.2f);
    glVertex2f(0.8f, +1.2f);

    glColor3f(0, 0, 1); // the second box is blue
    glVertex2f(0, -1.2f);
    glVertex2f(0.8f, +1.2f);
    glVertex2f(1.0, -1.1f);
    glVertex2f(2.0f, +1.4f);
    glColor3f(0.0f, 1.0f, 0.0f); // all the rest are green
    glVertex2f(1.0, -1.1f);
    glVertex2f(2.0f, +1.4f);
    glVertex2f(+3.1f, -1.0f);
    glVertex2f(+3.9f, +1.1f);
    glEnd();
    glPopMatrix();


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