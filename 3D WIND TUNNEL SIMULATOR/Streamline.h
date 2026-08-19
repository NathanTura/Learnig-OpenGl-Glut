#ifndef STREAMLINE_H
#define STREAMLINE_H

#include <vector>

struct Vec3 {
    float x;
    float y;
    float z;
};

class Streamline {
private:
    std::vector<Vec3> points;
    std::vector<Vec3> velocities;

public:
    // Generate a streamline starting at (startX, startY, startZ)
    void generate(float startX, float startY, float startZ, float stepSize, int maxSteps);
    
    // Draw the streamline
    void draw(bool useHeatmap, float windSpeed, bool useDisturbanceHeatmap = false, bool showPressure = false);
};

#endif
