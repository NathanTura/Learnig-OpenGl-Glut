#include "display.h"
#include "camera.h"
#include "map.h"
#include "input.h"
#include <GL/freeglut.h>

void display() {
    updateMovement();
    
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f); // Sky blue
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    updateCameraView();
    drawMap();
    
    glutSwapBuffers();
}
