#include "Streamline.h"
#include "WindField.h"
#include <GL/freeglut.h>
#include <cmath>

void Streamline::generate(float startX, float startY, float startZ, float stepSize, int maxSteps) {
    points.clear();
    
    Vec3 currentPos = {startX, startY, startZ};
    points.push_back(currentPos);
    
    for (int i = 0; i < maxSteps; i++) {
        // Stop generating if it leaves the wind tunnel
        if (currentPos.x > 10.0f) break;
        
        // Ask the WindField what the velocity is at this specific point
        WindSample wind = getWindVelocity(currentPos.x, currentPos.y, currentPos.z);
        
        // Calculate the speed (magnitude of velocity vector)
        float speed = std::sqrt(wind.vx*wind.vx + wind.vy*wind.vy + wind.vz*wind.vz);
        if (speed < 0.0001f) break; // Stop if the wind completely dies
        
        // Normalize the direction so we only move exactly `stepSize` distance
        float dirX = wind.vx / speed;
        float dirY = wind.vy / speed;
        float dirZ = wind.vz / speed;
        
        // Move along the velocity vector
        currentPos.x += dirX * stepSize;
        currentPos.y += dirY * stepSize;
        currentPos.z += dirZ * stepSize;
        
        // Save the new point
        points.push_back(currentPos);
    }
}

void Streamline::draw() {
    if (points.empty()) return;
    
    // GL_LINE_STRIP connects all vertices together in a continuous line
    glBegin(GL_LINE_STRIP);
    glColor3f(0.2f, 0.8f, 0.2f); // Green streamlines
    
    for (size_t i = 0; i < points.size(); i++) {
        glVertex3f(points[i].x, points[i].y, points[i].z);
    }
    
    glEnd();
}
