#ifndef SPHERE_H
#define SPHERE_H

#include "../Object.h"
#include <GL/freeglut.h>
#include <cmath>

class SphereObject : public TestObject {
public:
    float radius;
    float cx, cy, cz; // Center position of the sphere
    
    SphereObject(float r, float x, float y, float z) : radius(r), cx(x), cy(y), cz(z) {}
    
    void draw() override {
        glPushMatrix();
        glTranslatef(cx, cy, cz); // Move to the sphere's position
        glColor3f(0.8f, 0.2f, 0.2f); // Red
        glutSolidSphere(radius, 32, 32);
        glPopMatrix();
    }
    
    void applyDeflection(float px, float py, float pz, float& vx, float& vy, float& vz) override {
        // 1. Calculate vector from sphere center to the particle
        float dx = px - cx;
        float dy = py - cy;
        float dz = pz - cz;
        
        // 2. Calculate distance from center
        float dist = std::sqrt(dx*dx + dy*dy + dz*dz);
        
        // Define how far away the air starts feeling the sphere (e.g. 2.5x the radius)
        float influenceRadius = radius * 2.5f; 
        
        // 3. Detect whether the particle is close enough to be influenced
        if (dist < influenceRadius && dist > 0.0001f) {
            
            // Save original speed so we can normalize later
            float originalSpeed = std::sqrt(vx*vx + vy*vy + vz*vz);
            
            // Normalize the direction vector (make its length 1)
            dx /= dist;
            dy /= dist;
            dz /= dist;
            
            // Calculate how strong the push should be. 
            // It gets stronger the closer the particle is to the sphere (1.0 at center, 0.0 at edge)
            float strength = 1.0f - (dist / influenceRadius);
            
            // 4. Apply a repelling/deflection force pushing outward from the sphere
            float repelForce = strength * originalSpeed * 2.0f; 
            
            vx += dx * repelForce;
            vy += dy * repelForce;
            vz += dz * repelForce;
            
            // 5. Normalize the velocity
            // This ensures the wind bends around the object but doesn't suddenly speed up infinitely
            float newSpeed = std::sqrt(vx*vx + vy*vy + vz*vz);
            if (newSpeed > 0.0001f) {
                vx = (vx / newSpeed) * originalSpeed;
                vy = (vy / newSpeed) * originalSpeed;
                vz = (vz / newSpeed) * originalSpeed;
            }
        }
    }
};

#endif
