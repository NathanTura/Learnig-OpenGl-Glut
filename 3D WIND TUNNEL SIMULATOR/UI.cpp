#include "UI.h"
#include <GL/freeglut.h>
#include <sstream>

void UI::renderText(float x, float y, const std::string& text, int windowWidth, int windowHeight) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f); // White text
    glRasterPos2f(x, y);
    for (size_t i = 0; i < text.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void UI::drawHUD(float windSpeed, int currentObjectType, int windowWidth, int windowHeight,
                  bool useHeatmap, bool showPressure) {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    std::string speedText = "Wind Speed: " + std::to_string((int)windSpeed) + " m/s";
    std::string objText = "Object: ";
    if (currentObjectType == 0) objText += "Sphere";
    else if (currentObjectType == 1) objText += "Cube (Rankine Oval)";
    else if (currentObjectType == 2) objText += "Cylinder";
    else if (currentObjectType == 3) objText += "Airfoil";
    
    renderText(10.0f, windowHeight - 25.0f, speedText, windowWidth, windowHeight);
    renderText(10.0f, windowHeight - 50.0f, objText, windowWidth, windowHeight);
    
    if (showPressure) {
        renderText(10.0f, windowHeight - 75.0f,
                   "PRESSURE Cp:  RED (High/Stagnation) -> GREEN (Free-stream) -> BLUE (Suction)",
                   windowWidth, windowHeight);
    } else if (useHeatmap) {
        renderText(10.0f, windowHeight - 75.0f,
                   "VELOCITY:  BLUE (Low) -> GREEN (Med) -> RED (High)",
                   windowWidth, windowHeight);
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}
