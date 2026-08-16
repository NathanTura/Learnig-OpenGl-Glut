#include "Camera.h"
#include <GL/freeglut.h>
#include <cmath>

Camera::Camera(float startX, float startY, float startZ) {
    x = startX;
    y = startY;
    z = startZ;
    yaw = -90.0f; // Look towards -Z initially
    pitch = -15.0f; // Look slightly down
    firstMouse = true;
    lastMouseX = 0;
    lastMouseY = 0;
}

void Camera::reset() {
    x = 0.0f;
    y = 5.0f;
    z = 15.0f;
    yaw = -90.0f;
    pitch = -15.0f;
}

void Camera::getFrontVector(float& fx, float& fy, float& fz) {
    // Convert spherical angles to Cartesian coordinates
    float radYaw = yaw * (3.14159f / 180.0f);
    float radPitch = pitch * (3.14159f / 180.0f);

    fx = std::cos(radYaw) * std::cos(radPitch);
    fy = std::sin(radPitch);
    fz = std::sin(radYaw) * std::cos(radPitch);
}

void Camera::processKeyboard(unsigned char key, float deltaTime) {
    // We increase speed slightly so it feels responsive
    float speed = 10.0f * deltaTime; 
    
    float fx, fy, fz;
    getFrontVector(fx, fy, fz);
    
    // Calculate Right vector for A/D movement (Cross product of Front and World Up (0,1,0))
    float rx = fy * 0.0f - fz * 1.0f;
    float ry = 0.0f; 
    float rz = fx * 1.0f - fy * 0.0f;
    
    // Normalize Right vector
    float rLen = std::sqrt(rx*rx + ry*ry + rz*rz);
    if (rLen > 0) { rx /= rLen; ry /= rLen; rz /= rLen; }

    switch (key) {
        case 'w': case 'W': 
            x += fx * speed; y += fy * speed; z += fz * speed; 
            break;
        case 's': case 'S': 
            x -= fx * speed; y -= fy * speed; z -= fz * speed; 
            break;
        case 'a': case 'A': 
            x -= rx * speed; y -= ry * speed; z -= rz * speed; 
            break;
        case 'd': case 'D': 
            x += rx * speed; y += ry * speed; z += rz * speed; 
            break;
        case 'q': case 'Q': 
            y -= speed; // Global down
            break; 
        case 'e': case 'E': 
            y += speed; // Global up
            break; 
        case 'r': case 'R': 
            reset(); 
            break;
    }
}

void Camera::processMouseMovement(int mouseX, int mouseY) {
    if (firstMouse) {
        lastMouseX = mouseX;
        lastMouseY = mouseY;
        firstMouse = false;
        return;
    }

    float xOffset = (mouseX - lastMouseX);
    float yOffset = (lastMouseY - mouseY); // Reversed since window Y goes top-to-bottom
    lastMouseX = mouseX;
    lastMouseY = mouseY;

    float sensitivity = 0.2f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    // Constrain pitch to prevent the camera from flipping upside down
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

void Camera::applyViewMatrix() {
    float fx, fy, fz;
    getFrontVector(fx, fy, fz);
    
    // gluLookAt(eyeX, eyeY, eyeZ,  centerX, centerY, centerZ,  upX, upY, upZ)
    gluLookAt(x, y, z, 
              x + fx, y + fy, z + fz, 
              0.0f, 1.0f, 0.0f);
}
