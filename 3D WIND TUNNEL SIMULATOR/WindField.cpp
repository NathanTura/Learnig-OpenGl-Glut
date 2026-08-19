#include "WindField.h"
#include "Object.h"

extern TestObject* currentObject; // Get access to the object from main.cpp

float windSpeed = 5.0f; // Initial wind speed

WindSample getWindVelocity(float x, float y, float z) {
    WindSample sample;
    sample.vx = windSpeed; 
    sample.vy = 0.0f;
    sample.vz = 0.0f;
    if (currentObject) {
        currentObject->applyDeflection(x, y, z, sample.vx, sample.vy, sample.vz);
    }
    return sample;
}

bool isInsideCurrentObject(float x, float y, float z) {
    if (!currentObject) return false;
    return currentObject->isInsideSolid(x, y, z);
}

void pushOutOfCurrentObject(float x, float y, float z, float& outX, float& outY, float& outZ) {
    if (!currentObject) { outX=x; outY=y; outZ=z; return; }
    currentObject->pushOut(x, y, z, outX, outY, outZ);
}

