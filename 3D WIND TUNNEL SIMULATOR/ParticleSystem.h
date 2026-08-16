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
    
    void resetParticle(Particle& p, bool initialSpawn);
    void updateParticles(float deltaTime);
    void drawParticles();
};

#endif
