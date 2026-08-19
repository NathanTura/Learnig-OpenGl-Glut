#include "ParticleSystem.h"
#include "WindField.h"
#include <GL/freeglut.h>
#include <cstdlib>
#include <cmath>
#include "Utils.h"

ParticleSystem::ParticleSystem(int count) {
    maxParticles = count;
    particles.resize(maxParticles);
    
    // Initialize particles spread out across the entire tunnel
    // so the simulator doesn't start with a single block of air.
    resetAll();
}

void ParticleSystem::resetAll() {
    for (int i = 0; i < maxParticles; i++) {
        resetParticle(particles[i], true); // true = spread them out across the tunnel
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

        // 3b. If the particle stepped inside the solid, push it back out
        if (isInsideCurrentObject(p.x, p.y, p.z)) {
            float ox, oy, oz;
            pushOutOfCurrentObject(p.x, p.y, p.z, ox, oy, oz);
            p.x = ox; p.y = oy; p.z = oz;
        }

        // Prevent clipping through the floor and ceiling
        if (p.y < 0.1f) p.y = 0.1f;
        if (p.y > 3.9f) p.y = 3.9f;

        // Prevent clipping through the front and back walls
        if (p.z > -0.1f) p.z = -0.1f;
        if (p.z < -3.9f) p.z = -3.9f;

        // 4. Reset particle if it leaves the outlet
        if (p.x > 10.0f) {
            resetParticle(p, false); // false = spawn at inlet
        }
    }
}

void ParticleSystem::drawParticles(bool useHeatmap, float windSpeed, bool useDisturbanceHeatmap, bool showPressure) {
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (const auto& p : particles) {
        if (showPressure) {
            float speed = std::sqrt(p.vx*p.vx + p.vy*p.vy + p.vz*p.vz);
            applyPressureColor(speed, windSpeed);
        } else if (useHeatmap) {
            if (useDisturbanceHeatmap) {
                float dx = p.vx - windSpeed;
                float dist = std::sqrt(dx*dx + p.vy*p.vy + p.vz*p.vz);
                applyHeatmapColor(dist, windSpeed, true);
            } else {
                float speed = std::sqrt(p.vx*p.vx + p.vy*p.vy + p.vz*p.vz);
                applyHeatmapColor(speed, windSpeed, false);
            }
        } else {
            glColor3f(1.0f, 1.0f, 1.0f);
        }
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
    glPointSize(1.0f);
}
