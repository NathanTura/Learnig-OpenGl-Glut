#include "input.h"
#include "camera.h"
#include <GL/freeglut.h>

bool keys[256] = {false};
int lastMouseX = 400;
int lastMouseY = 300;
bool firstMouse = true;

void keyboardDown(unsigned char key, int x, int y) {
    keys[key] = true;
    if (key == 27) { // Escape key to exit
        exit(0);
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

void mouseMotion(int x, int y) {
    if (firstMouse) {
        lastMouseX = x;
        lastMouseY = y;
        firstMouse = false;
    }
    
    float xoffset = x - lastMouseX;
    float yoffset = lastMouseY - y; 
    
    lastMouseX = x;
    lastMouseY = y;
    
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    rotateCamera(xoffset, yoffset);
}

void updateMovement() {
    float speed = 0.05f;
    if (keys['w'] || keys['W']) moveCameraForward(speed);
    if (keys['s'] || keys['S']) moveCameraForward(-speed);
    if (keys['a'] || keys['A']) moveCameraRight(-speed);
    if (keys['d'] || keys['D']) moveCameraRight(speed);
}
