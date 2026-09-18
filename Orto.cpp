#include <GL/freeglut.h>
#include <iostream>
float angle = -20.0f;

void timer(int value) {
    angle += 10.0f;
    if (angle > 180.0f) {
        angle = -20.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(100, timer, 0);
}

void drawViewport1() {
    // Viewport 1: Bottom-Left Quarter (x=0, y=0, width=250, height=250)
    glViewport(0, 0, 250, 250);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 50.0, -10.0, 40.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw rotated blue rectangle
    glColor3f(0.0f, 0.0f, 1.0f);
    glPushMatrix();
    // Translate to the center of the bounding box (25, 15) to rotate in place
    glTranslatef(25.0f, 15.0f, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    // Draw the rectangle centered around the origin so it spins on its axis
    glRectf(-5.0f, -15.0f, 5.0f, 15.0f);
    glPopMatrix();

    // Draw bounding box outline
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.0f, 40.0f);
        glVertex2f(50.0f, 40.0f);
        glVertex2f(50.0f, -10.0f);
        glVertex2f(0.0f, -10.0f);
    glEnd();
}

void drawViewport2() {
    // Viewport 2: Top-Right Quarter (x=250, y=250, width=250, height=250)
    glViewport(250, 250, 250, 250);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 50.0, -10.0, 40.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw bounding box outline
    glColor3f(0.0f, 0.0f, 0.0f);
 
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.0f, 40.0f);
        glVertex2f(50.0f, 40.0f);
        glVertex2f(50.0f, -10.0f);
        glVertex2f(0.0f, -10.0f);
    glEnd();

    // Draw red rectangle
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix(); // ADDED missing push matrix
    // Translate to the center of the bounding box (25, 15) to rotate in place
    glTranslatef(25.0f, 15.0f, 0.0f);
    glRotatef(-angle, 0.0f, 0.0f, 1.0f);
    // Draw the rectangle centered around the origin so it spins on its axis
    glRectf(-5.0f, -15.0f, 5.0f, 15.0f);
    glPopMatrix(); // ADDED missing pop matrix
}

void display() {
    // Yellow background color
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawViewport1();
    drawViewport2();

    glFlush();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Multi-Viewport OpenGL");

    glutDisplayFunc(display);
    glutTimerFunc(100, timer, 0);

    glutMainLoop();
    return 0;
}