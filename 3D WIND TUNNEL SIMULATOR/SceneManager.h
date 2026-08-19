#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Object.h"
#include "ParticleSystem.h"
#include "Streamline.h"
#include "Camera.h"
#include <vector>

class SceneManager {
public:
    ParticleSystem* particleSystem;
    Camera* mainCamera;
    std::vector<Streamline> streamlines;
    
    int currentObjectType; // 0=Sphere, 1=Cube, 2=Cylinder, 3=Airfoil
    int visualizationMode; // 1=Particles, 2=Streamlines, 3=Both
    
    bool showVectors;
    float vectorStep;
    bool useHeatmap;
    bool useDisturbanceHeatmap;
    bool showDebug;
    bool showPressure;  // Phase 18: Pressure field (Cp)

    SceneManager();
    ~SceneManager();

    void init();
    void update(float deltaTime);
    void draw();
    void drawVectors();
    void printDebugInfo();
    
    void cycleObject();
    void changeWindSpeed(float amount);
    void setVisualizationMode(int mode);
    
private:
    void generateStreamlines();
};

#endif
