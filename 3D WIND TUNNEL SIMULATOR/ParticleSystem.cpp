#include "ParticleSystem.h"
#include "WindField.h"
#include <GL/freeglut.h>
#include <cstdlib>

ParticleSystem::ParticleSystem(int count) {
    maxParticles = count;
    particles.resize(maxParticles);
    
    // Initialize particles spread out across the entire tunnel
    // so the simulator doesn't start with a single block of air.
    for (int i = 0; i < maxParticles; i++) {
        resetParticle(particles[i], true);
    }
}

// Resets a particle to the inlet when it flies out of bounds
void ParticleSystem::resetParticle(Particle& p, bool initialSpawn) {
    // If it's the very first spawn when the program opens, spread them along the X axis
    // Otherwise, spawn them exactly at the Inlet (X = -10.0)
    if (initialSpawn) {
        p.x = -10.0f + (static_cast<float>(rand()) / RAND_MAX) * 20.0f;
    } else {
        p.x = -10.0f;
    }
    
    // Randomize Y and Z so the flow is continuous and not identical
    p.y = 0.1f + (static_cast<float>(rand()) / RAND_MAX) * 3.8f;
    p.z = -3.9f + (static_cast<float>(rand()) / RAND_MAX) * 3.8f;
    
    p.vx = 0.0f;
    p.vy = 0.0f;
    p.vz = 0.0f;
    
    // Randomize lifetime slightly in case we want fading later
    p.lifetime = 0.5f + (static_cast<float>(rand()) / RAND_MAX) * 1.0f;
}

void ParticleSystem::updateParticles(float deltaTime) {
    for (int i = 0; i < maxParticles; i++) {
        Particle& p = particles[i];
        
        // 1. Get the wind velocity at the particle's current position
        WindSample wind = getWindVelocity(p.x, p.y, p.z);
        
        // 2. Set particle velocity to match the wind
        p.vx = wind.vx;
        p.vy = wind.vy;
        p.vz = wind.vz;
        
        // 3. Move the particle
        p.x += p.vx * deltaTime;
        p.y += p.vy * deltaTime;
        p.z += p.vz * deltaTime;

        // 4. Reset particle if it leaves the outlet
        if (p.x > 10.0f) {
            resetParticle(p, false); // false = spawn at inlet
        }
    }
}

void ParticleSystem::drawParticles() {
    glPointSize(3.0f); // Make particles slightly larger than a single pixel
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f); // White particles
    
    for (int i = 0; i < maxParticles; i++) {
        glVertex3f(particles[i].x, particles[i].y, particles[i].z);
    }
    
    glEnd();
    glPointSize(1.0f); // Reset point size
}
