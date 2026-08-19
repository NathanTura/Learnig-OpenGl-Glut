#ifndef CYLINDER_H
#define CYLINDER_H

#include "../Object.h"
#include <GL/freeglut.h>
#include <cmath>

class CylinderObject : public TestObject {
public:
    float radius;
    float height;
    float cx, cy, cz;
    
    CylinderObject(float r, float h, float x, float y, float z) 
        : radius(r), height(h), cx(x), cy(y), cz(z) {}
    
    void draw() override {
        glPushMatrix();
        glTranslatef(cx, cy, cz);
        
        // Cylinder is drawn along Z axis by default in glutSolidCylinder, 
        // we want it vertical (along Y axis)
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        
        glColor3f(0.8f, 0.8f, 0.2f); // Yellow
        glutSolidCylinder(radius, height, 32, 32);
        glPopMatrix();
    }
    
    void applyDeflection(float px, float py, float pz, float& vx, float& vy, float& vz) override {
        float dx = px - cx;
        // Float dy = py - cy is not strictly needed for 2D flow in X-Z plane
        float dz = pz - cz;
        
        // 2D distance squared in the X-Z plane
        float r2 = dx*dx + dz*dz;
        float r = std::sqrt(r2);
        
        // Check if inside the bounds of the cylinder
        if (r < radius && py >= cy && py <= cy + height) {
            vx += (dx / r) * 10.0f;
            vz += (dz / r) * 10.0f;
            return;
        }
        
        // Exact 2D Potential Flow past a Cylinder
        // Free stream velocity (U) is in the X direction (vx)
        float U = vx;
        
        float R2 = radius * radius;
        float r4 = r2 * r2;
        
        // Add the doublet perturbation for a 2D cylinder
        // u = U * R^2 * (z^2 - x^2) / r^4
        // w = -U * R^2 * (2*x*z) / r^4
        
        float perturbation_x = U * R2 * (dz*dz - dx*dx) / r4;
        float perturbation_z = -U * R2 * (2.0f * dx * dz) / r4;
        
        vx += perturbation_x;
        // vy is undisturbed by a vertical cylinder
        vz += perturbation_z;
    }
};

#endif
