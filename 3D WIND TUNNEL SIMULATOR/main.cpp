#include <GL/freeglut.h>
#include <iostream>
#include "SceneManager.h"
#include "UI.h"
#include "WindField.h" // For global windSpeed access in UI

SceneManager* scene = nullptr;

bool keys[256] = {false};
bool isDragging = false;
int lastTime = 0;
int windowWidth = 800;
int windowHeight = 600;

void init() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    scene = new SceneManager();
    scene->init();
    
    lastTime = glutGet(GLUT_ELAPSED_TIME);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (scene && scene->mainCamera) {
        scene->mainCamera->applyViewMatrix();
        scene->draw();
        
        extern float windSpeed;
        UI::drawHUD(windSpeed, scene->currentObjectType, windowWidth, windowHeight,
                    scene->useHeatmap, scene->showPressure);
    }

    glutSwapBuffers();
}

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

void keyboardDown(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
    
    if (scene) {
        if (key == '1') scene->setVisualizationMode(1);
        if (key == '2') scene->setVisualizationMode(2);
        if (key == '3') scene->setVisualizationMode(3);
        
        if (key == '+' || key == '=') scene->changeWindSpeed(1.0f);
        if (key == '-' || key == '_') scene->changeWindSpeed(-1.0f);
        if (key == 'o' || key == 'O') scene->cycleObject();
        if (key == 'v' || key == 'V') scene->showVectors = !scene->showVectors;
        if (key == 'h' || key == 'H') scene->useHeatmap = !scene->useHeatmap;
        if (key == 'm' || key == 'M') scene->useDisturbanceHeatmap = !scene->useDisturbanceHeatmap;
        if (key == 'p' || key == 'P') scene->showPressure = !scene->showPressure;
        if (key == 'd' || key == 'D') {
            scene->showDebug = !scene->showDebug;
            if (scene->showDebug) scene->printDebugInfo();
        }
        if (key == '[') {
            scene->vectorStep += 0.25f;
            if (scene->vectorStep > 3.0f) scene->vectorStep = 3.0f;
        }
        if (key == ']') {
            scene->vectorStep -= 0.25f;
            if (scene->vectorStep < 0.25f) scene->vectorStep = 0.25f;
        }
    }
    
    keys[key] = true;
}

void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            if (scene && scene->mainCamera) scene->mainCamera->firstMouse = true;
        } else if (state == GLUT_UP) {
            isDragging = false;
        }
    }
}

void mouseMotion(int x, int y) {
    if (isDragging && scene && scene->mainCamera) {
        scene->mainCamera->processMouseMovement(x, y);
        glutPostRedisplay();
    }
}

void update() {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    if (deltaTime > 0.1f) deltaTime = 0.1f;

    if (scene) {
        scene->update(deltaTime);
        
        // Continuous camera movement
        if (keys['w'] || keys['W']) scene->mainCamera->processKeyboard('w', deltaTime);
        if (keys['s'] || keys['S']) scene->mainCamera->processKeyboard('s', deltaTime);
        if (keys['a'] || keys['A']) scene->mainCamera->processKeyboard('a', deltaTime);
        if (keys['d'] || keys['D']) scene->mainCamera->processKeyboard('d', deltaTime);
        if (keys['q'] || keys['Q']) scene->mainCamera->processKeyboard('q', deltaTime);
        if (keys['e'] || keys['E']) scene->mainCamera->processKeyboard('e', deltaTime);
        if (keys['r'] || keys['R']) scene->mainCamera->reset();
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Virtual 3D Wind Tunnel - Refactored");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutIdleFunc(update);

    std::cout << "Controls:\n";
    std::cout << "W/S/A/D: Move (relative to where you look)\n";
    std::cout << "Q/E: Move Down/Up\n";
    std::cout << "Left Click + Drag: Look around\n";
    std::cout << "R: Reset Camera\n";
    std::cout << "1/2/3: Visualization Modes\n";
    std::cout << "V: Toggle Velocity Vectors\n";
    std::cout << "H: Toggle Velocity Heatmap\n";
    std::cout << "P: Toggle Pressure Field (Cp)\n";
    std::cout << "M: Toggle Disturbance/Absolute Heatmap Mode\n";
    std::cout << "D: Toggle Debug Info\n";
    std::cout << "[/]: Decrease/Increase Vector Density\n";
    std::cout << "+/-: Wind Speed\n";
    std::cout << "O: Cycle Object\n";
    std::cout << "ESC: Exit\n";

    glutMainLoop();

    delete scene;
    return 0;
}
