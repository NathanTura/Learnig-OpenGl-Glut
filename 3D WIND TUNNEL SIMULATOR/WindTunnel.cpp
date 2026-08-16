#include "WindTunnel.h"
#include <GL/freeglut.h>

// Draw a simple grid on the XZ plane
void drawGrid() {
    glBegin(GL_LINES);
    glColor3f(0.5f, 0.5f, 0.5f); // Gray color
    
    for (float i = -20.0f; i <= 20.0f; i += 1.0f) {
        glVertex3f(-20.0f, -0.01f, i);
        glVertex3f(20.0f, -0.01f, i);
        glVertex3f(i, -0.01f, -20.0f);
        glVertex3f(i, -0.01f, 20.0f);
    }
    glEnd();
}

// Draw the coordinate system axes
void drawAxes() {
    glLineWidth(3.0f); // Make lines thicker
    glBegin(GL_LINES);
    
    // X Axis (Red) - Airflow direction
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(3.0f, 0.0f, 0.0f);

    // Y Axis (Green) - Vertical
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 3.0f, 0.0f);

    // Z Axis (Blue) - Depth
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 3.0f);
    
    glEnd();
    glLineWidth(1.0f); // Reset line width
}

// Draw the transparent Wind Tunnel
void drawWindTunnel() {
    // Tunnel bounds
    float minX = -10.0f, maxX = 10.0f; 
    float minY = 0.0f,   maxY = 4.0f;  
    float minZ = -4.0f,  maxZ = 0.0f;  

    // 1. Draw solid floor
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.3f); 
    glVertex3f(minX, minY, maxZ);
    glVertex3f(maxX, minY, maxZ);
    glVertex3f(maxX, minY, minZ);
    glVertex3f(minX, minY, minZ);
    glEnd();

    // Enable blending for transparent walls
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    // 2. Draw semi-transparent walls
    glBegin(GL_QUADS);
    glColor4f(0.4f, 0.7f, 1.0f, 0.2f); 

    // Ceiling
    glVertex3f(minX, maxY, maxZ);
    glVertex3f(maxX, maxY, maxZ);
    glVertex3f(maxX, maxY, minZ);
    glVertex3f(minX, maxY, minZ);

    // Back Wall
    glVertex3f(minX, minY, minZ);
    glVertex3f(maxX, minY, minZ);
    glVertex3f(maxX, maxY, minZ);
    glVertex3f(minX, maxY, minZ);
    glEnd();

    // 3. Draw wireframe outlines for clarity
    glBegin(GL_LINES);
    glColor4f(0.8f, 0.8f, 1.0f, 0.6f); 

    // Ceiling edges
    glVertex3f(minX, maxY, minZ); glVertex3f(maxX, maxY, minZ);
    glVertex3f(minX, maxY, maxZ); glVertex3f(maxX, maxY, maxZ);
    glVertex3f(minX, maxY, minZ); glVertex3f(minX, maxY, maxZ);
    glVertex3f(maxX, maxY, minZ); glVertex3f(maxX, maxY, maxZ);

    // Floor edges
    glVertex3f(minX, minY, minZ); glVertex3f(maxX, minY, minZ);
    glVertex3f(minX, minY, maxZ); glVertex3f(maxX, minY, maxZ);

    // Corner pillars
    glVertex3f(minX, minY, minZ); glVertex3f(minX, maxY, minZ);
    glVertex3f(maxX, minY, minZ); glVertex3f(maxX, maxY, minZ);
    glVertex3f(minX, minY, maxZ); glVertex3f(minX, maxY, maxZ);
    glVertex3f(maxX, minY, maxZ); glVertex3f(maxX, maxY, maxZ);

    glEnd();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}
