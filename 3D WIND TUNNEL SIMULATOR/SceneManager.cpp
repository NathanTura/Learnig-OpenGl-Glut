#include "SceneManager.h"
#include "Objects/Sphere.h"
#include "Objects/Cube.h"
#include "Objects/Cylinder.h"
#include "Objects/Airfoil.h"
#include "WindField.h"
#include "WindTunnel.h"
#include <GL/freeglut.h>
#include "Utils.h"

// Global pointer needed by WindField.cpp to calculate deflection
TestObject* currentObject = nullptr;

SceneManager::SceneManager() {
    particleSystem = nullptr;
    mainCamera = nullptr;
    currentObjectType = 0;
    visualizationMode = 3;
    showVectors = false;
    vectorStep = 1.0f;
    useHeatmap = false;
    useDisturbanceHeatmap = false;
    showDebug = false;
    showPressure = false;
}

SceneManager::~SceneManager() {
    delete currentObject;
    delete particleSystem;
    delete mainCamera;
}

void SceneManager::init() {
    mainCamera = new Camera(0.0f, 5.0f, 15.0f);
    currentObject = new SphereObject(1.0f, 0.0f, 2.0f, -2.0f);
    particleSystem = new ParticleSystem(1000);
    generateStreamlines();
    
    // Setup Lighting
    glEnable(GL_COLOR_MATERIAL); 
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH); 

    GLfloat lightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat lightDiffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPosition[] = { 5.0f, 10.0f, 10.0f, 1.0f }; 

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void SceneManager::generateStreamlines() {
    streamlines.clear();
    for (float y = 0.5f; y < 4.0f; y += 0.5f) {
        for (float z = -3.5f; z < 0.0f; z += 0.5f) {
            Streamline sl;
            sl.generate(-10.0f, y, z, 0.1f, 500); 
            streamlines.push_back(sl);
        }
    }
}

void SceneManager::update(float deltaTime) {
    if (particleSystem) {
        particleSystem->updateParticles(deltaTime);
    }
}

void SceneManager::draw() {
    // Draw the environment (unlit)
    drawGrid();       
    drawAxes();       
    drawWindTunnel(); 

    // Draw the test object with 3D Lighting enabled
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    if (currentObject) {
        currentObject->draw();
    }
    glDisable(GL_LIGHTING);

    // Draw the particles
    extern float windSpeed;
    
    if (visualizationMode == 1 || visualizationMode == 3) {
        if (particleSystem) {
            particleSystem->drawParticles(useHeatmap, windSpeed, useDisturbanceHeatmap, showPressure);
        }
    }

    // Draw the streamlines
    if (visualizationMode == 2 || visualizationMode == 3) {
        glLineWidth(2.0f);
        for (size_t i = 0; i < streamlines.size(); i++) {
            streamlines[i].draw(useHeatmap, windSpeed, useDisturbanceHeatmap, showPressure);
        }
        glLineWidth(1.0f);
    }
    
    // Draw velocity vectors
    if (showVectors) {
        drawVectors();
    }
}

void SceneManager::drawVectors() {
    extern float windSpeed;
    
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    
    for (float x = -10.0f; x <= 10.0f; x += vectorStep) {
        for (float y = 0.5f; y <= 4.0f; y += vectorStep) {
            for (float z = -4.0f; z <= 0.0f; z += vectorStep) {
                float vx = windSpeed;
                float vy = 0.0f;
                float vz = 0.0f;
                
                if (currentObject) {
                    currentObject->applyDeflection(x, y, z, vx, vy, vz);
                }
                
                float speed = std::sqrt(vx*vx + vy*vy + vz*vz);
                if (speed < 0.001f) continue;
                
                if (useHeatmap || showPressure) {
                    if (showPressure) {
                        applyPressureColor(speed, windSpeed);
                    } else if (useDisturbanceHeatmap) {
                        float dx = vx - windSpeed;
                        float dist = std::sqrt(dx*dx + vy*vy + vz*vz);
                        applyHeatmapColor(dist, windSpeed, true);
                    } else {
                        applyHeatmapColor(speed, windSpeed, false);
                    }
                } else {
                    glColor3f(1.0f, 1.0f, 0.0f);
                }
                
                float scale = 0.05f; // Scale vector length for visibility
                
                glVertex3f(x, y, z);
                glVertex3f(x + vx * scale, y + vy * scale, z + vz * scale);
            }
        }
    }
    glEnd();
}

void SceneManager::cycleObject() {
    currentObjectType = (currentObjectType + 1) % 4; // Now cycles through 4 objects
    
    delete currentObject;
    if (currentObjectType == 0) {
        currentObject = new SphereObject(1.0f, 0.0f, 2.0f, -2.0f);
    } else if (currentObjectType == 1) {
        currentObject = new CubeObject(1.0f, 0.0f, 2.0f, -2.0f);
    } else if (currentObjectType == 2) {
        currentObject = new CylinderObject(1.0f, 4.0f, 0.0f, 0.0f, -2.0f);
    } else if (currentObjectType == 3) {
        currentObject = new AirfoilObject(3.0f, 0.5f, 0.0f, 2.0f, -2.0f); // chord=3.0, thickness=0.5
    }
    
    generateStreamlines();
    if (particleSystem) particleSystem->resetAll();
}

void SceneManager::changeWindSpeed(float amount) {
    extern float windSpeed; // from WindField.h
    windSpeed += amount;
    if (windSpeed < 1.0f) windSpeed = 1.0f;
    
    generateStreamlines();
    if (particleSystem) particleSystem->resetAll();
}

void SceneManager::setVisualizationMode(int mode) {
    visualizationMode = mode;
}

void SceneManager::printDebugInfo() {
    extern float windSpeed;
    printf("\n=== SCENE DEBUG ===\n");
    printf("WindSpeed: %.2f m/s\n", windSpeed);
    printf("VisualizationMode: %d\n", visualizationMode);
    printf("Heatmap: %s  Disturbance: %s  Vectors: %s\n",
        useHeatmap?"ON":"OFF",
        useDisturbanceHeatmap?"ON":"OFF",
        showVectors?"ON":"OFF");

    // Dispatch to object-specific debug if it's an airfoil
    AirfoilObject* af = dynamic_cast<AirfoilObject*>(currentObject);
    if (af) {
        af->printDebug(windSpeed);
    } else {
        printf("Current object is not an Airfoil.\n");
    }
    printf("===================\n\n");
}

