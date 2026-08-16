#include <GL/freeglut.h>
#include <iostream>
#include "display.h"
#include "input.h"
#include "camera.h"

using namespace std;

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = w * 1.0 / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("3D Game");
	
	glEnable(GL_DEPTH_TEST);
	
	initCamera();
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutPassiveMotionFunc(mouseMotion);
	glutMotionFunc(mouseMotion);

	// Hide cursor and warp to center could be added here, but keeping it simple
	// glutSetCursor(GLUT_CURSOR_NONE);

	glutMainLoop();
	return 0;
}