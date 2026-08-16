#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:
    float x, y, z;
    float yaw, pitch; 
    
    bool firstMouse;
    int lastMouseX, lastMouseY;

    Camera(float startX, float startY, float startZ);

    void processKeyboard(unsigned char key, float deltaTime);
    void processMouseMovement(int mouseX, int mouseY);
    void reset();

    void getFrontVector(float& fx, float& fy, float& fz);
    void applyViewMatrix();
};

#endif
