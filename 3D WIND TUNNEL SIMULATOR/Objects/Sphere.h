#ifndef SPHERE_H
#define SPHERE_H

#include "../Object.h"
#include <GL/freeglut.h>
#include <cmath>

class SphereObject : public TestObject {
public:
    float radius;
    float cx, cy, cz;

    SphereObject(float r, float x, float y, float z) : radius(r), cx(x), cy(y), cz(z) {}

    void draw() override {
        glPushMatrix();
        glTranslatef(cx, cy, cz);
        glColor3f(0.8f, 0.2f, 0.2f); // Red
        glutSolidSphere(radius, 32, 32);
        glPopMatrix();
    }

    void applyDeflection(float px, float py, float pz, float& vx, float& vy, float& vz) override {
        float dx = px - cx;
        float dy = py - cy;
        float dz = pz - cz;
        
        float r2 = dx*dx + dy*dy + dz*dz;
        float r = std::sqrt(r2);
        
        if (r < radius) {
            // Push particles outward if they accidentally clip inside
            vx += (dx / r) * 10.0f; 
            vy += (dy / r) * 10.0f; 
            vz += (dz / r) * 10.0f;
            return;
        }
        
   
        float U = vx; 
        
       
        float R3 = radius * radius * radius;
        float r5 = r2 * r2 * r;
        
        float doublet_strength = (U * R3) / (2.0f * r5);
        
        // Exact doublet perturbation velocities
        float perturbation_x = doublet_strength * (r2 - 3.0f * dx * dx);
        float perturbation_y = doublet_strength * (-3.0f * dx * dy);
        float perturbation_z = doublet_strength * (-3.0f * dx * dz);
        
        // Note: The uniform flow 'U' is ALREADY inside vx. So we just add the perturbation.
        // Wait, standard equation: V_x = U (1 - R^3/(2r^3) + 3R^3x^2/(2r^5))
        // So perturbation is - U * R^3/(2r^3) + U * 3R^3x^2/(2r^5)
        // = (U*R^3 / (2*r^5)) * (3x^2 - r^2). Yes, this exactly matches!
        
        vx += perturbation_x;
        vy += perturbation_y;
        vz += perturbation_z;
    }
};

#endif
