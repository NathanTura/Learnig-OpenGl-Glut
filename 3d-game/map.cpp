#include "map.h"
#include <GL/freeglut.h>

void drawMap() {
    glColor3f(0.3f, 0.8f, 0.3f);
    glBegin(GL_QUADS);
    glVertex3f(-50.0f, 0.0f, -50.0f);
    glVertex3f(-50.0f, 0.0f,  50.0f);
    glVertex3f( 50.0f, 0.0f,  50.0f);
    glVertex3f( 50.0f, 0.0f, -50.0f);
    glEnd();
    
     glColor3f(0.0f, 0.0f, 0.0f);
     glBegin(GL_LINES);
     for(int i=-50; i<=50; i+=2) {
         glVertex3f((float)i, 0.01f, -50.0f);
         glVertex3f((float)i, 0.01f, 50.0f);
        
         glVertex3f(-50.0f, 0.01f, (float)i);
         glVertex3f( 50.0f, 0.01f, (float)i);
     }
     glEnd();
}
