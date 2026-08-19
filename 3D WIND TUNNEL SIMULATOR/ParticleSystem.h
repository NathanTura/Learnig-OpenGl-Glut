#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include "Particle.h"

class ParticleSystem {
private:
    std::vector<Particle> particles;
    int maxParticles;

public:
    ParticleSystem(int count);
    
    void resetAll();
    void resetParticle(Particle& p, bool initialSpawn);
    void updateParticles(float deltaTime);
    void drawParticles(bool useHeatmap, float maxSpeed, bool useDisturbanceHeatmap = false, bool showPressure = false);
};

#endif
