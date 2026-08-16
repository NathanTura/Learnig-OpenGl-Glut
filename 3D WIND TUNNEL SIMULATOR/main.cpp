#include <GL/freeglut.h>
#include <iostream>
#include "WindTunnel.h"
#include "WindField.h"
#include "Object.h"
#include "Objects/Sphere.h"
#include "ParticleSystem.h"
#include "Streamline.h"
#include "Camera.h"
#include <vector>

// ==========================================
// GLOBALS
// ==========================================
TestObject* currentObject = nullptr;
ParticleSystem* particleSystem = nullptr;
std::vector<Streamline> streamlines;
Camera* mainCamera = nullptr;
int lastTime = 0;
bool isDragging = false;
bool keys[256] = {false};
int visualizationMode = 3; // 1 = Particles, 2 = Streamlines, 3 = Both


int windowWidth = 800;
int windowHeight = 600;

// Initialize OpenGL state
void init() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Setup Camera
    mainCamera = new Camera(0.0f, 5.0f, 15.0f);

    // Initialize our test object (a sphere with radius 1.0 at X=0, Y=2, Z=-2)
    currentObject = new SphereObject(1.0f, 0.0f, 2.0f, -2.0f);

    // Initialize 1000 particles
    particleSystem = new ParticleSystem(1000);
    
    // Generate Streamlines starting from the inlet
    for (float y = 0.5f; y < 4.0f; y += 0.5f) {
        for (float z = -3.5f; z < 0.0f; z += 0.5f) {
            Streamline sl;
            // Generate streamline: startX, startY, startZ, stepSize, maxSteps
            sl.generate(-10.0f, y, z, 0.1f, 500); 
            streamlines.push_back(sl);
        }
    }
    
    // Initialize time for physics updates
    lastTime = glutGet(GLUT_ELAPSED_TIME);
}

// Render callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Use our new Camera class to set the view matrix
    if (mainCamera) {
        mainCamera->applyViewMatrix();
    }

    // Draw the scene
    drawGrid();       
    drawAxes();       
    drawWindTunnel(); 

    // Draw the test object (It now handles its own position)
    if (currentObject) {
        currentObject->draw();
    }

    // Draw the particles
    if (visualizationMode == 1 || visualizationMode == 3) {
        if (particleSystem) {
            particleSystem->drawParticles();
        }
    }

    // Draw the streamlines
    if (visualizationMode == 2 || visualizationMode == 3) {
        glLineWidth(2.0f); // Make them slightly thicker
        for (size_t i = 0; i < streamlines.size(); i++) {
            streamlines[i].draw();
        }
        glLineWidth(1.0f); // Reset line width
    }

    glutSwapBuffers();
}

// Window resize callback
void reshape(int w, int h) {
    if (h == 0) h = 1; 
    windowWidth = w;
    windowHeight = h;
    float ratio = w * 1.0f / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
}

// Keyboard input callback (Key Pressed)
void keyboardDown(unsigned char key, int x, int y) {
    if (key == 27) { // ESC key
        exit(0);
    }
    
    // Visualization modes
    if (key == '1') visualizationMode = 1;
    if (key == '2') visualizationMode = 2;
    if (key == '3') visualizationMode = 3;
    
    // Record that this key is currently being held down
    keys[key] = true;
}

// Keyboard input callback (Key Released)
void keyboardUp(unsigned char key, int x, int y) {
    // Record that this key has been released
    keys[key] = false;
}

// Mouse button callback
void mouseButton(int button, int state, int x, int y) {
    // Only rotate camera if left-clicking and dragging
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            if (mainCamera) mainCamera->firstMouse = true; // prevent jump
        } else if (state == GLUT_UP) {
            isDragging = false;
        }
    }
}

// Mouse movement callback
void mouseMotion(int x, int y) {
    if (isDragging && mainCamera) {
        mainCamera->processMouseMovement(x, y);
        glutPostRedisplay();
    }
}

void update() {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (currentTime - lastTime) / 1000.0f; // Convert ms to seconds
    lastTime = currentTime;

    // Prevent massive jumps if the window is dragged or frozen
    if (deltaTime > 0.1f) deltaTime = 0.1f;

    if (particleSystem) {
        particleSystem->updateParticles(deltaTime);
    }

    // Process continuous camera movement
    if (mainCamera) {
        if (keys['w'] || keys['W']) mainCamera->processKeyboard('w', deltaTime);
        if (keys['s'] || keys['S']) mainCamera->processKeyboard('s', deltaTime);
        if (keys['a'] || keys['A']) mainCamera->processKeyboard('a', deltaTime);
        if (keys['d'] || keys['D']) mainCamera->processKeyboard('d', deltaTime);
        if (keys['q'] || keys['Q']) mainCamera->processKeyboard('q', deltaTime);
        if (keys['e'] || keys['E']) mainCamera->processKeyboard('e', deltaTime);
        if (keys['r'] || keys['R']) mainCamera->reset();
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Virtual 3D Wind Tunnel - Phase 4");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp); // Register key release callback
    glutMouseFunc(mouseButton); // New mouse click callback
    glutMotionFunc(mouseMotion); // New mouse drag callback
    glutIdleFunc(update);

    std::cout << "Controls:" << std::endl;
    std::cout << "W/S/A/D: Move (relative to where you look)" << std::endl;
    std::cout << "Q/E: Move Down/Up" << std::endl;
    std::cout << "Left Click + Drag: Look around" << std::endl;
    std::cout << "R: Reset Camera" << std::endl;
    std::cout << "1: Particles Only" << std::endl;
    std::cout << "2: Streamlines Only" << std::endl;
    std::cout << "3: Both Modes" << std::endl;
    std::cout << "ESC: Exit" << std::endl;

    glutMainLoop();

    delete currentObject;
    delete particleSystem;
    delete mainCamera;
    return 0;
}
