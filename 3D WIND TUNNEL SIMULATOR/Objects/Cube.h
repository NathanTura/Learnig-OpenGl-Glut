#ifndef CUBE_H
#define CUBE_H

#include "../Object.h"
#include <GL/freeglut.h>
#include <cmath>

class CubeObject : public TestObject {
public:
    float size;
    float cx, cy, cz;
    
    CubeObject(float s, float x, float y, float z) : size(s), cx(x), cy(y), cz(z) {}
    
    void draw() override {
        glPushMatrix();
        glTranslatef(cx, cy, cz);
        glColor3f(0.2f, 0.8f, 0.2f); // Green
        glutSolidCube(size * 2.0f);
        glPopMatrix();
    }
    
    void applyDeflection(float px, float py, float pz, float& vx, float& vy, float& vz) override {
        float dx = px - cx;
        float dy = py - cy;
        float dz = pz - cz;
        
        // Inside the cube
        if (std::abs(dx) < size && std::abs(dy) < size && std::abs(dz) < size) {
            // Push outwards based on which face is closest
            vx += dx * 5.0f;
            vy += dy * 5.0f;
            vz += dz * 5.0f;
            return;
        }
        
        // 3D Rankine Body (Source + Sink + Uniform Flow)
        // This mathematically simulates a blunt, block-like object in fluid
        
        float U = vx;
        
        // Distance from center to source/sink
        float a = size * 0.8f; 
        // Strength of the source/sink. Scale by U and size to match the cube volume.
        float m = U * size * size * 1.5f; 
        
        // Source is at dx = -a (upstream), Sink is at dx = +a (downstream)
        float dx_source = dx + a;
        float r1_2 = dx_source*dx_source + dy*dy + dz*dz;
        float r1 = std::sqrt(r1_2);
        float r1_3 = r1_2 * r1;
        
        float dx_sink = dx - a;
        float r2_2 = dx_sink*dx_sink + dy*dy + dz*dz;
        float r2 = std::sqrt(r2_2);
        float r2_3 = r2_2 * r2;
        
        // Calculate velocity perturbations from source and sink
        float perturbation_x = m * (dx_source / r1_3 - dx_sink / r2_3);
        float perturbation_y = m * dy * (1.0f / r1_3 - 1.0f / r2_3);
        float perturbation_z = m * dz * (1.0f / r1_3 - 1.0f / r2_3);
        
        vx += perturbation_x;
        vy += perturbation_y;
        vz += perturbation_z;
    }
};

#endif
