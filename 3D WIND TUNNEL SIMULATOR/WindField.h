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

// Returns true if point is inside the current solid object
bool isInsideCurrentObject(float x, float y, float z);

// Fills outX/Y/Z with a corrected position outside the current solid object
void pushOutOfCurrentObject(float x, float y, float z, float& outX, float& outY, float& outZ);

#endif
