#include "WindField.h"
#include "Object.h"

extern TestObject* currentObject; // Get access to the object from main.cpp

float windSpeed = 5.0f; // Initial wind speed

WindSample getWindVelocity(float x, float y, float z) {
    WindSample sample;
    
    // Base wind velocity
    sample.vx = windSpeed; 
    sample.vy = 0.0f;
    sample.vz = 0.0f;
    
    // If we have an object in the tunnel, ask it to deflect the airflow
    if (currentObject) {
        currentObject->applyDeflection(x, y, z, sample.vx, sample.vy, sample.vz);
    }
    
    return sample;
}
