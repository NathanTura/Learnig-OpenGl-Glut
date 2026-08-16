#include "camera.h"
#include <GL/freeglut.h>
#include <math.h>

float camX = 0.0f, camY = 1.0f, camZ = 5.0f;
float camYaw = -90.0f, camPitch = 0.0f;
float camFrontX = 0.0f, camFrontY = 0.0f, camFrontZ = -1.0f;

void updateCameraVectors() {
    float radYaw = camYaw * 3.14159f / 180.0f;
    float radPitch = camPitch * 3.14159f / 180.0f;
    camFrontX = cos(radYaw) * cos(radPitch);
    camFrontY = sin(radPitch);
    camFrontZ = sin(radYaw) * cos(radPitch);
    
    float length = sqrt(camFrontX*camFrontX + camFrontY*camFrontY + camFrontZ*camFrontZ);
    if(length != 0.0f) {
        camFrontX /= length;
        camFrontY /= length;
        camFrontZ /= length;
    }
}

void initCamera() {
    updateCameraVectors();
}

void updateCameraView() {
    gluLookAt(camX, camY, camZ,
              camX + camFrontX, camY + camFrontY, camZ + camFrontZ,
              0.0f, 1.0f, 0.0f);
}

void moveCameraForward(float speed) {
    camX += camFrontX * speed;
    camZ += camFrontZ * speed;
}

void moveCameraRight(float speed) {
    float rightX = -camFrontZ;
    float rightZ = camFrontX;
    float length = sqrt(rightX*rightX + rightZ*rightZ);
    if(length != 0.0f) {
        rightX /= length;
        rightZ /= length;
    }
    
    camX += rightX * speed;
    camZ += rightZ * speed;
}

void rotateCamera(float dYaw, float dPitch) {
    camYaw += dYaw;
    camPitch += dPitch;
    if (camPitch > 89.0f) camPitch = 89.0f;
    if (camPitch < -89.0f) camPitch = -89.0f;
    updateCameraVectors();
}
