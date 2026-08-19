#ifndef OBJECT_H
#define OBJECT_H

class TestObject {
public:
    virtual ~TestObject() {}
    virtual void draw() = 0;
    
    // Calculates how this specific object deflects the airflow around it
    // Takes the particle's position (px,py,pz) and modifies its velocity (vx,vy,vz)
    virtual void applyDeflection(float px, float py, float pz, float& vx, float& vy, float& vz) = 0;
    
    // Returns true if the given point is inside the solid geometry
    virtual bool isInsideSolid(float px, float py, float pz) { return false; }
    
    // Returns a push-out vector to move a point outside the solid
    virtual void pushOut(float px, float py, float pz, float& outX, float& outY, float& outZ) {
        outX = px; outY = py; outZ = pz; // default: no change
    }
};

#endif
