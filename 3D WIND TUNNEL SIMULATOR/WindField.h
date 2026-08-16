#ifndef WINDFIELD_H
#define WINDFIELD_H

struct WindSample {
    float vx;
    float vy;
    float vz;
};

// Global wind speed
extern float windSpeed;

// Returns the velocity of the wind at a specific point in space
WindSample getWindVelocity(float x, float y, float z);

#endif
