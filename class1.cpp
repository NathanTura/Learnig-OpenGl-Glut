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


void displayp()
{
    glColor3f(0.0f, 1.0f, 0.0f);

    glBegin(GL_POLYGON);

        glVertex3f(-2.0f, 0.05f, -1.0f);
        glVertex3f( 0.0f, 0.05f, -2.0f);
        glVertex3f( 2.0f, 0.05f, -1.0f);
        glVertex3f( 2.0f, 0.05f,  1.0f);
        glVertex3f( 0.0f, 0.05f,  2.0f);
        glVertex3f(-2.0f, 0.05f,  1.0f);


        

    glEnd();
}

void rendertriangle()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        10.0f, 8.0f, 15.0f,   // Camera position
        0.0f, 2.0f, 0.0f,     // Look at
        0.0f, 1.0f, 0.0f      // Up
    );

    glPushMatrix();

        glColor3f(0.3f, 0.3f, 0.3f);

        glBegin(GL_QUADS);

            glVertex3f(-8.0f, 0.0f, -4.0f);
            glVertex3f( 8.0f, 0.0f, -4.0f);
            glVertex3f( 8.0f, 0.0f,  4.0f);
            glVertex3f(-8.0f, 0.0f,  4.0f);

        glEnd();

    glPopMatrix();

    glPushMatrix();

        displayp();

    glPopMatrix();

    glPushMatrix();

      
        glTranslatef(-4.0f, 0.0f, 0.0f);

     
        glRotatef(angle, 0.0f, 1.0f, 0.0f);

        glColor3f(1.0f, 0.0f, 0.0f);

        glBegin(GL_TRIANGLES);

            // Front
            glVertex3f(0.0f, 2.0f, 0.0f);
            glVertex3f(-2.0f, 0.0f, 2.0f);
            glVertex3f(2.0f, 0.0f, 2.0f);

            // Right
            glVertex3f(0.0f, 2.0f, 0.0f);
            glVertex3f(2.0f, 0.0f, 2.0f);
            glVertex3f(2.0f, 0.0f, -2.0f);

            // Back
            glVertex3f(0.0f, 2.0f, 0.0f);
            glVertex3f(2.0f, 0.0f, -2.0f);
            glVertex3f(-2.0f, 0.0f, -2.0f);

            // Left
            glVertex3f(0.0f, 2.0f, 0.0f);
            glVertex3f(-2.0f, 0.0f, -2.0f);
            glVertex3f(-2.0f, 0.0f, 2.0f);

        glEnd();

    glPopMatrix();


    glPushMatrix();


        glTranslatef(4.0f, 2.5f, 0.0f);

        glRotatef(angle, 0.0f, 1.0f, 0.0f);

        glColor3f(0.0f, 0.0f, 1.0f);

        glBegin(GL_QUADS);

            // Front
            glVertex3f(-2.5f, -2.5f,  2.5f);
            glVertex3f( 2.5f, -2.5f,  2.5f);
            glVertex3f( 2.5f,  2.5f,  2.5f);
            glVertex3f(-2.5f,  2.5f,  2.5f);

            // Back
            glVertex3f( 2.5f, -2.5f, -2.5f);
            glVertex3f(-2.5f, -2.5f, -2.5f);
            glVertex3f(-2.5f,  2.5f, -2.5f);
            glVertex3f( 2.5f,  2.5f, -2.5f);

            // Right
            glVertex3f(2.5f, -2.5f,  2.5f);
            glVertex3f(2.5f, -2.5f, -2.5f);
            glVertex3f(2.5f,  2.5f, -2.5f);
            glVertex3f(2.5f,  2.5f,  2.5f);

            // Left
            glVertex3f(-2.5f, -2.5f, -2.5f);
            glVertex3f(-2.5f, -2.5f,  2.5f);
            glVertex3f(-2.5f,  2.5f,  2.5f);
            glVertex3f(-2.5f,  2.5f, -2.5f);

            // Top
            glVertex3f(-2.5f, 2.5f,  2.5f);
            glVertex3f( 2.5f, 2.5f,  2.5f);
            glVertex3f( 2.5f, 2.5f, -2.5f);
            glVertex3f(-2.5f, 2.5f, -2.5f);

            // Bottom
            glVertex3f(-2.5f, -2.5f, -2.5f);
            glVertex3f( 2.5f, -2.5f, -2.5f);
            glVertex3f( 2.5f, -2.5f,  2.5f);
            glVertex3f(-2.5f, -2.5f,  2.5f);

        glEnd();

    glPopMatrix();


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

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_DEPTH |
        GLUT_RGBA
    );

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);

    glutCreateWindow("3D Objects on Platform");

    glutDisplayFunc(rendertriangle);
    glutReshapeFunc(reshape);
    glutIdleFunc(update);

    glEnable(GL_DEPTH_TEST);


    glClearColor(
        0.1f,
        0.1f,
        0.1f,
        1.0f
    );


    glutMainLoop();

    return 0;
}