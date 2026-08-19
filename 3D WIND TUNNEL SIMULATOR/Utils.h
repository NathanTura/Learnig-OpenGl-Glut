#ifndef UTILS_H
#define UTILS_H

#include <GL/freeglut.h>

inline void applyHeatmapColor(float speed, float windSpeed, bool isDisturbance = false) {
    float ratio = speed / windSpeed;
    float r = 0.0f, g = 0.0f, b = 0.0f;
    
    if (isDisturbance) {
        // Disturbance visualization
        if (ratio < 0.25f) {
            b = 1.0f;
            g = ratio / 0.25f;
        } else if (ratio < 0.5f) {
            b = 1.0f - ((ratio - 0.25f) / 0.25f);
            g = 1.0f;
        } else if (ratio < 0.75f) {
            g = 1.0f;
            r = (ratio - 0.5f) / 0.25f;
        } else {
            g = std::max(0.0f, 1.0f - ((ratio - 0.75f) / 0.25f));
            r = 1.0f;
        }
    } else {
        // Absolute velocity
        if (ratio <= 0.70f) {
            b = 1.0f;
        } else if (ratio < 0.90f) {
            b = 1.0f;
            g = (ratio - 0.70f) / 0.20f;
        } else if (ratio < 1.10f) {
            b = 1.0f - ((ratio - 0.90f) / 0.20f);
            g = 1.0f;
        } else if (ratio < 1.30f) {
            g = 1.0f;
            r = (ratio - 1.10f) / 0.20f;
        } else if (ratio < 1.50f) {
            g = 1.0f - ((ratio - 1.30f) / 0.20f) * 0.5f; // Orange
            r = 1.0f;
        } else {
            r = 1.0f;
        }
    }
    
    glColor3f(r, g, b);
}

// Pressure coefficient: Cp = 1 - (v/U)^2
// Cp +1  = stagnation = RED   (high pressure)
// Cp  0  = free stream = GREEN
// Cp -1  = suction     = BLUE  (low pressure)
inline void applyPressureColor(float speed, float windSpeed) {
    float Cp = 1.0f - (speed / windSpeed) * (speed / windSpeed);
    // Clamp to [-2, +1]
    if (Cp >  1.0f) Cp =  1.0f;
    if (Cp < -2.0f) Cp = -2.0f;

    float r = 0.f, g = 0.f, b = 0.f;
    if (Cp >= 0.0f) {
        // Green (Cp=0) -> Red (Cp=+1)
        r = Cp;
        g = 1.0f - Cp;
    } else if (Cp >= -1.0f) {
        // Cyan (Cp=-0.5) -> Blue (Cp=-1)
        float t = -Cp;          // 0..1
        b = t;
        g = 1.0f - t;
    } else {
        // Deep violet for extreme suction (Cp < -1)
        float t = (-Cp - 1.0f); // 0..1
        b = 1.0f;
        r = t * 0.5f;
    }
    glColor3f(r, g, b);
}
#endif
