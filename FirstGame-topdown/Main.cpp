#include <GL/freeglut.h>
#include "drawmap.h"
#include "HandlePlayer.h"

void reshape(int w, int h)
{
    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0, w, 0, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    drawMap();
    player.draw();

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);

    glutCreateWindow("Top Down Test");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutSpecialFunc(handleMovement);

    glutMainLoop();

    return 0;
}