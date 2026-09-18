#include <GL/freeglut.h>
#include <iostream>

float angleX = 0.0f;

void rotate()
{
    angleX += 0.5f; 

    if (angleX >= 360.0f)
        angleX = 0.0f;

    glutPostRedisplay();
}
void initLighting()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
}

void drawPyramid()
{   

    glBegin(GL_TRIANGLES);

    glColor3f(0.0f, 0.0f, 1.0f); 
    glNormal3f(0.0f, 0.447f, 0.894f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glColor3f(0.0f, 1.0f, 0.0f); 
    glNormal3f(0.894f, 0.447f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glColor3f(1.0f, 0.0f, 0.0f); 
    glNormal3f(0.0f, 0.447f, -0.894f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);


    glColor3f(0.5f, 0.0f, 0.5f); 
    glNormal3f(-0.894f, 0.447f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(1.0f, 1.0f, 0.0f); 
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(
        45.0,
        800.0 / 600.0,
        0.1,
        100.0
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        3.0f, 3.0f, 5.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );

    GLfloat light[] = { 3.0f, 4.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light);

    glPushMatrix();

    glRotatef(angleX, 1, 0, 0);

    drawPyramid();

    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Light");
    
    initLighting();
    
    glutDisplayFunc(display);
    glutIdleFunc(rotate);
    
    glutMainLoop();

    return 0;
}