#include <GL/freeglut.h>
#include <iostream>
#include <cmath>

float playerX = 0.0f;       
float playerY = -0.8f;     
float playerWidth = 0.3f;   
float playerHeight = 0.05f; 

void drawBall(float radius, int segments)
{
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(0.0f, 0.0f);

    for (int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * 3.14159f * i / segments;

        float x = radius * cos(angle);
        float y = radius * sin(angle);

        glVertex2f(x, y);
    }

    glEnd();
}

void drawPlayer() {
    glPushMatrix();
    
    glTranslatef(playerX, playerY, 0.0f);
    
    glColor3f(0.2f, 0.5f, 1.0f);
    
    glBegin(GL_QUADS);
        glVertex2f(-playerWidth / 2.0f, -playerHeight / 2.0f); // Bottom-left
        glVertex2f( playerWidth / 2.0f, -playerHeight / 2.0f); // Bottom-right
        glVertex2f( playerWidth / 2.0f,  playerHeight / 2.0f); // Top-right
        glVertex2f(-playerWidth / 2.0f,  playerHeight / 2.0f); // Top-left
    glEnd();
    
    glPopMatrix();

    glPushMatrix();

    glColor3f(1.0f, 0.0f, 0.0f);

    drawBall(0.025f, 80);


    
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawPlayer();

    glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { 
        exit(0);
    }
 
    if (key == 'a' || key == 'A') { playerX -= 0.1f; }
    if (key == 'd' || key == 'D') { playerX += 0.1f; }
    
    glutPostRedisplay();
}


void specialKeys(int key, int x, int y) {  

    if (key == GLUT_KEY_LEFT) { playerX -= 0.1f; }
    if (key == GLUT_KEY_RIGHT) { playerX += 0.1f; }

    glutPostRedisplay();
}

void init() {
  
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

int main(int argc, char** argv) {
  
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Breakout - Player");
    
    init();
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    glutMainLoop();   
    return 0;
}
