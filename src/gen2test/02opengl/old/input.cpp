#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

using namespace std;
void render_scene_cb() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1,0,0);
    glBegin(GL_TRIANGLES);
    glVertex3f(0,0,0);
    glVertex3f(1,0,0);
    glVertex3f(1,1,0);
    glEnd();
    glFlush();
    glutSwapBuffers();
    cerr << "render done" << flush;
}


void keyboard_cb(unsigned char key, int mouse_x, int mouse_y) {
//    glutPostRedisplay();
    cerr << "Key=" << int(key) << ' ' << flush;
}

void special_keyboard_cb(int key, int mouse_x, int mouse_y) {
    glutPostRedisplay();    
    cerr << "SKey:" << int(key) << '\n';
}


void passive_mouse_cb(int x, int y) {
    cerr << "mouse=" << x << "," << y << flush;
}

void init() {
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    const int x = 200, y = 100, w = 1024, h = 800;
    glutInitWindowSize(w, h);
    glutInitWindowPosition(x, y);
    int win = glutCreateWindow("glut input demo");

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) throw runtime_error("glewInit");
    glClearColor(0,0,0,1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);

    glutDisplayFunc(render_scene_cb);
    glutKeyboardFunc(keyboard_cb);
    glutSpecialFunc(special_keyboard_cb);
    glutPassiveMotionFunc(passive_mouse_cb);
}

int main(int argc, char* argv[]) {
    try {
        glutInit(&argc, argv);
        init();
        glutMainLoop();
   } catch (exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}