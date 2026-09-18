#include <GL/freeglut.h>
#include <iostream>
#include <cmath>

using namespace std;

void cube();
void polygon(int, int, int, int);

float rotx = 0;
float roty = 0;
float rotz = 0;

GLfloat vertices[][3] = {
    {-1.0, -1.0,  1.0}, {-1.0,  1.0,  1.0},
    { 1.0,  1.0,  1.0}, { 1.0, -1.0,  1.0},
    {-1.0, -1.0, -1.0}, {-1.0,  1.0, -1.0},
    { 1.0,  1.0, -1.0}, { 1.0, -1.0, -1.0}
};



void keypress(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        rotx += 5;
    }
    if (key == GLUT_KEY_DOWN) {
        rotx -= 5;
    }
    if (key == GLUT_KEY_RIGHT) {
        roty += 5;
    }
    if (key == GLUT_KEY_LEFT) {
        roty -= 5;
    }
    glutPostRedisplay();
}

void polygon(int a, int b, int c, int d) {
  
    glBegin(GL_POLYGON);
    glVertex3fv(vertices[a]);
    glVertex3fv(vertices[b]);
    glVertex3fv(vertices[c]);
    glVertex3fv(vertices[d]);
    glEnd();
}

void cube() {
    polygon(0, 3, 2, 1);
    polygon(3, 7, 6, 2);
    polygon(7, 4, 5, 6);
    polygon(4, 0, 1, 5);
    polygon(1, 2, 6, 5);
    polygon(4, 7, 3, 0);
}

void drawHouse() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    gluLookAt(
        0.0, 4.0, 12.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0
    );


    glRotatef(rotx, 1.0, 0.0, 0.0);
    glRotatef(roty, 0.0, 1.0, 0.0);
    glRotatef(rotz, 0.0, 0.0, 1.0);

 
    glPushMatrix();
       glColor3f(0.6f, 0.3f, 0.0f);
        glTranslatef(0.0f, -1.2f, 0.0f); 
        glScalef(4.0f, 0.2f, 4.0f);      
        cube();
    glPopMatrix();

    
    glPushMatrix();
        glColor3f(1.0f , .4f , 1.0f);
        cube();
    glPopMatrix();

 
    glPushMatrix();
        glTranslatef(0.0f, 1.0f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); 
        glColor3f(0.8f, 0.2f, 0.2f); 
        glutSolidCone(1.5, 1.5, 20, 20); 
    glPopMatrix();

 
    glPushMatrix();
            glColor3f(.9f, 0.3f, 0.0f);
        glTranslatef(-2.5f, -0.5f, 2.5f); 
        glScalef(0.2f, 0.5f, 0.2f); 
        cube();
    glPopMatrix();


    glPushMatrix();
        glColor3f(0.0f , 1.0f , 0.0f);
        glTranslatef(-2.5f, 0.5f, 2.5f);
        glScalef(0.8f, 0.8f, 0.8f); 
        cube();
    glPopMatrix();


    glPushMatrix();  glColor3f(.9f, 0.3f, 0.0f);
        glTranslatef(3.0f, -0.5f, -1.5f);
        glScalef(0.2f, 0.5f, 0.2f); 
        cube();
    glPopMatrix();


    glPushMatrix();
       glColor3f(0.0f , 1.0f , 0.0f);
        glTranslatef(3.0f, 0.5f, -1.5f);
        glScalef(0.6f, 0.6f, 0.6f); 
        cube();
    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);

    glutCreateWindow("House");

    glutDisplayFunc(drawHouse);
    glutSpecialFunc(keypress); 
    
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    return 0;
}