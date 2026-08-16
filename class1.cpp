#include <GL/freeglut.h>
#include <iostream>
#include <cmath>


void reshape(int width, int height)
{
    if (height == 0)
        height = 1;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(
        60.0,
        (double)width / (double)height,
        0.1,
        1000.0
    );

    glMatrixMode(GL_MODELVIEW);
}


float angle = 0.0f;


void rendertriangle()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    gluLookAt(
        0.0f, 5.0f, 10.0f,   // Camera position: X, Y, Z
        0.0f, 0.0f, 0.0f,    // Point the camera is looking at
        0.0f, 1.0f, 0.0f     // Up direction
    );


    // Rotate the object around the Y axis
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    // glRotatef(angle, 1.0f, 0.0f, 0.0f);
    // glRotatef(angle, 0.0f, 1.0f, 1.0f);

     


    glBegin(GL_QUADS);

        glVertex3f(0.0f , 0.0f , 0.0f);
        glVertex3f(5.0f , 0.0f , 0.0f);
        glVertex3f(5.0f , 5.0f , 0.0f);
        glVertex3f(0.0f , 5.0f , 0.0f);

        glVertex3f(5.0f , 5.0f , 0.0f);
        glVertex3f(5.0f , 5.0f , 5.0f);
        glVertex3f(5.0f , 0.0f , 5.0f);
        glVertex3f(5.0f , 0.0f , 0.0f);

        glVertex3f(0.0f , 5.0f , 0.0f);
        glVertex3f(0.0f , 5.0f , 5.0f);
        glVertex3f(0.0f , 0.0f , 5.0f);
        glVertex3f(0.0f , 0.0f , 0.0f);

        glVertex3f(5.0f , 0.0f , 0.0f);
        glVertex3f(5.0f , 5.0f , 0.0f);
        glVertex3f(0.0f , 5.0f , 0.0f);
        glVertex3f(0.0f , 0.0f , 0.0f);

        glVertex3f(5.0f , 5.0f , 0.0f);
        glVertex3f(5.0f , 5.0f , 5.0f);
        glVertex3f(0.0f , 5.0f , 5.0f);
        glVertex3f(0.0f , 5.0f , 0.0f);

        glVertex3f(5.0f , 0.0f , 0.0f);
        glVertex3f(5.0f , 0.0f , 5.0f);
        glVertex3f(0.0f , 0.0f , 5.0f);
        glVertex3f(0.0f , 0.0f , 0.0f);


    glEnd();

    glutSwapBuffers();
}



void update()
{
     angle += 0.5f;

     if (angle >= 360.0f)
         angle = 0.0f;

     glutPostRedisplay();
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

    glutInitWindowPosition(800, 600);
    glutInitWindowSize(800, 600);

    glutCreateWindow("Triangle");

    glutDisplayFunc(rendertriangle);
    glutReshapeFunc(reshape);

    // Continuously update the rotation
    glutIdleFunc(update);

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    return 1;
}