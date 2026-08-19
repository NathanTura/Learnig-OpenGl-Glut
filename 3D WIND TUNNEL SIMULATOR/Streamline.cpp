#include "Streamline.h"
#include "WindField.h"
#include <GL/freeglut.h>
#include <cmath>
#include "Utils.h"

void Streamline::generate(float startX, float startY, float startZ, float stepSize, int maxSteps) {
    points.clear();
    velocities.clear();
    
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

        // If the new position is inside the solid, push it back out
        if (isInsideCurrentObject(currentPos.x, currentPos.y, currentPos.z)) {
            float ox, oy, oz;
            pushOutOfCurrentObject(currentPos.x, currentPos.y, currentPos.z, ox, oy, oz);
            currentPos.x = ox; currentPos.y = oy; currentPos.z = oz;
        }
        
        // Save the new point AND the velocity at that point
        Vec3 vel; vel.x = wind.vx; vel.y = wind.vy; vel.z = wind.vz;
        points.push_back(currentPos);
        velocities.push_back(vel);
    }
    // Pad velocities to match points (first point has no velocity yet)
    if (!velocities.empty() && velocities.size() < points.size()) {
        velocities.insert(velocities.begin(), velocities.front());
    }
}

void Streamline::draw(bool useHeatmap, float windSpeed, bool useDisturbanceHeatmap, bool showPressure) {
    if (points.empty()) return;
    glBegin(GL_LINE_STRIP);
    if (!useHeatmap && !showPressure) glColor3f(0.2f, 0.8f, 0.2f);
    
    for (size_t i = 0; i < points.size(); i++) {
        Vec3 v = (i < velocities.size()) ? velocities[i] : velocities.back();
        float speed = std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
        
        if (showPressure) {
            applyPressureColor(speed, windSpeed);
        } else if (useHeatmap) {
            if (useDisturbanceHeatmap) {
                float dx = v.x - windSpeed;
                float dist = std::sqrt(dx*dx + v.y*v.y + v.z*v.z);
                applyHeatmapColor(dist, windSpeed, true);
            } else {
                applyHeatmapColor(speed, windSpeed, false);
            }
        }
        glVertex3f(points[i].x, points[i].y, points[i].z);
    }
    glEnd();
}
