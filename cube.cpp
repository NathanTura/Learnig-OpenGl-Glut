#include <GL/freeglut.h>
#include <iostream>
#include <cmath>


float angle = 0.0f;


void rotate(){
    if (angle > 360.0f) {
        angle = 0.0f;
    }
    angle += 0.1f;

 
    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        5.0f , 5.0f, 5.0f ,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );
    
    glPushMatrix();

        glColor3f(0.2f, 0.7f, 1.0f);

        glBegin(GL_QUADS);

            glVertex3f(-3.0f, 0.0f, -4.0f);
            glVertex3f( 3.0f, 0.0f, -4.0f);
            glVertex3f( 3.0f, 0.0f,  4.0f);
            glVertex3f(-3.0f, 0.0f,  4.0f);

        glEnd();

    glPopMatrix();



    glPushMatrix();

        glTranslatef(1.5f, 1.0f, 0.5f);
        
        glRotatef(angle, 0.0f, 1.0f, 0.0f);


        glBegin(GL_QUADS);

        
             glColor3f(1.0f, 0.3f, 0.2f);
            glVertex3f(-0.5f, -0.5f,  0.5f);
            glVertex3f( 0.5f, -0.5f,  0.5f);
            glVertex3f( 0.5f,  0.5f,  0.5f);
            glVertex3f(-0.5f,  0.5f,  0.5f);

              glColor3f(0.0f, 0.3f, 0.2f);
            glVertex3f( 0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f,  0.5f, -0.5f);
            glVertex3f( 0.5f,  0.5f, -0.5f);
            
             glColor3f(0.0f, 1.0f, 0.2f);
            glVertex3f( 0.5f, -0.5f,  0.5f);
            glVertex3f( 0.5f, -0.5f, -0.5f);
            glVertex3f( 0.5f,  0.5f, -0.5f);
            glVertex3f( 0.5f,  0.5f,  0.5f);
            
             glColor3f(1.0f, 2.3f, 0.2f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f,  0.5f);
            glVertex3f(-0.5f,  0.5f,  0.5f);
            glVertex3f(-0.5f,  0.5f, -0.5f);

             glColor3f(1.0f, 0.3f, 1.2f);
            glVertex3f(-0.5f, 0.5f,  0.5f);
            glVertex3f( 0.5f, 0.5f,  0.5f);
            glVertex3f( 0.5f, 0.5f, -0.5f);
            glVertex3f(-0.5f, 0.5f, -0.5f);

             glColor3f(0.5f, 1.3f, 0.2f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f( 0.5f, -0.5f, -0.5f);
            glVertex3f( 0.5f, -0.5f,  0.5f);
            glVertex3f(-0.5f, -0.5f,  0.5f);

        glEnd();

    glPopMatrix();

    glPushMatrix();


    glTranslatef(0.0f, 1.0f, 2.0f);
    
    glRotatef(angle, 0.0f, 1.0f, 0.0f);


    glColor3f(1.0f, 0.0f, 0.0f);

    glutSolidSphere(0.5, 100, 32);

    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cube");

    glutDisplayFunc(display);
    
    glutIdleFunc(rotate);

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    return 0;
}