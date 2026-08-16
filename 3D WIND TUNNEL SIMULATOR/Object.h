#ifndef OBJECT_H
#define OBJECT_H

class TestObject {
public:
    virtual ~TestObject() {}
    virtual void draw() = 0;
    
    // Calculates how this specific object deflects the airflow around it
    // Takes the particle's position (px,py,pz) and modifies its velocity (vx,vy,vz)
    virtual void applyDeflection(float px, float py, float pz, float& vx, float& vy, float& vz) = 0;
};

#endif
